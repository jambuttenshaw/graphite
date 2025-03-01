#include "graphite_d3d12_pch.h"
#include "D3D12GraphicsContext.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/CommandRecordingContext.h"
#include "Pipelines/D3D12GraphicsPipeline.h"
#include "Graphite/RHI/DescriptorAllocators.h"

#include "D3D12Types.h"
#include "D3D12Exceptions.h"
#include "D3D12CommandQueue.h"
#include "D3D12CommandRecordingContext.h"


namespace Graphite::D3D12
{

	D3D12GraphicsContext::D3D12GraphicsContext(const GraphicsContextDesc& contextDesc)
		: GraphicsContext(contextDesc)
		, m_NativeBackBufferFormat(GraphiteFormatToD3D12Format(contextDesc.BackBufferFormat))
	{
		// Validate description
		GRAPHITE_ASSERT(contextDesc.WindowHandle, "Invalid window handle.");
		GRAPHITE_ASSERT(contextDesc.BackBufferWidth && contextDesc.BackBufferHeight, "Invalid back buffer dimensions.");
		GRAPHITE_ASSERT(contextDesc.MaxRecordingContextsPerFrame, "Invalid MaxRecordingContextsPerFrame.");

		GRAPHITE_LOG_INFO("Creating D3D12 Graphics Context...");

		// Initialization
		CreateAdapter();
		CreateDevice();

		CreateCommandQueues();
		CreateSwapChain(contextDesc.WindowHandle);

		CreateDescriptorHeaps();
		CreateBackBufferRTVs();

		const uint32_t totalRecordingContexts = contextDesc.MaxRecordingContextsPerFrame + s_InternalCommandRecordingContexts;
		CreateFrameResources(totalRecordingContexts);
		CreateRecordingContexts(totalRecordingContexts);

		// In case any set-up work was performed by the graphics context
		m_FrameResources.at(m_CurrentBackBuffer).SetFence(m_DirectQueue->Signal());
		m_DirectQueue->WaitForIdleCPUBlocking();

		// Release temporary resources created during init
		ProcessAllDeferrals();

		GRAPHITE_LOG_INFO("D3D12 Graphics Context created successfully.");
	}

	D3D12GraphicsContext::~D3D12GraphicsContext()
	{
		// Wait for all work on GPU to complete before releasing resources
		D3D12GraphicsContext::WaitForGPUIdle();

		m_BackBufferRTVs.Free();

		ProcessAllDeferrals();
	}


	void D3D12GraphicsContext::BeginFrame()
	{
		// Reset allocators so they can be distributed when requested
		// TODO: Only reset allocators as they are requested to prevent resetting allocators unnecessarily
		m_FrameResources.at(m_CurrentBackBuffer).ResetAllAllocators();

		// Internal pass to perform setup
		BeginPass();
		{
			D3D12CommandRecordingContext* recordingContext = dynamic_cast<D3D12CommandRecordingContext*>(AcquireRecordingContext());

			// Transition back buffer to be used as a render target
			const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				m_BackBuffers.at(m_CurrentBackBuffer).Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			recordingContext->GetCommandList()->ResourceBarrier(1, &barrier);

			CloseRecordingContext(recordingContext);
		}
		EndPass();
	}

	void D3D12GraphicsContext::EndFrame()
	{
		// Ensure all recording contexts have been returned to the graphics context
		GRAPHITE_ASSERT(m_OpenRecordingContexts == 0, "Some recording contexts were not closed before EndFrame.");


		// Finally perform an internal pass that ends the frame
		BeginPass();
		{
			D3D12CommandRecordingContext* recordingContext = dynamic_cast<D3D12CommandRecordingContext*>(AcquireRecordingContext());

			// Transition back buffer to be used as a render target
			const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				m_BackBuffers.at(m_CurrentBackBuffer).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			recordingContext->GetCommandList()->ResourceBarrier(1, &barrier);

			CloseRecordingContext(recordingContext);
		}
		EndPass();

		m_RecordingContextsUsedThisFrame = 0;
	}

	void D3D12GraphicsContext::BeginPass()
	{
		// TODO: This will perform debug marking

		// TODO: Allow for COMMAND CONTEXTS to be re-used throughout multiple passes
		// TODO: COMMAND ALLOCATORS must remain strictly single use per frame, obviously
	}

	void D3D12GraphicsContext::EndPass()
	{
		GRAPHITE_ASSERT(m_OpenRecordingContexts == 0, "Some recording contexts were not closed before EndPass.");
		FlushPendingCommandLists();
	}

	CommandRecordingContext* D3D12GraphicsContext::AcquireRecordingContext()
	{
		std::lock_guard<std::mutex> lockGuard(m_RecordingContextAcquisitionMutex);

		GRAPHITE_ASSERT(m_RecordingContextsUsedThisFrame < (m_RecordingContexts.size() - s_InternalCommandRecordingContexts), "Exceeded application recording context limit.");

		D3D12FrameResources& frameResources = m_FrameResources.at(m_CurrentBackBuffer);

		D3D12CommandRecordingContext* recordingContext = &m_RecordingContexts.at(m_RecordingContextsUsedThisFrame++);
		recordingContext->Reset(frameResources.GetNextCommandAllocator());
		recordingContext->SetDescriptorHeaps(m_ResourceHeap.get(), m_SamplerHeap.get());

		m_OpenRecordingContexts++;
		return recordingContext;
	}

	void D3D12GraphicsContext::CloseRecordingContext(CommandRecordingContext* recordingContext)
	{
		std::lock_guard<std::mutex> lockGuard(m_RecordingContextAcquisitionMutex);

		D3D12CommandRecordingContext* nativeRecordingContext = dynamic_cast<D3D12CommandRecordingContext*>(recordingContext);
		GRAPHITE_ASSERT(recordingContext, "Nullptr recording context");

		nativeRecordingContext->Close();
		m_PendingCommandLists.push_back(nativeRecordingContext->GetCommandList());

		m_OpenRecordingContexts--;
	}


	void D3D12GraphicsContext::Present()
	{
		const UINT interval = m_VSync ? 1 : 0;
		const UINT flags = (m_TearingSupport && m_WindowedMode && !m_VSync) ? DXGI_PRESENT_ALLOW_TEARING : 0;

		const HRESULT result = m_SwapChain->Present(interval, flags);
		if (FAILED(result))
		{
			switch (result)
			{
			case DXGI_ERROR_DEVICE_RESET:
			case DXGI_ERROR_DEVICE_REMOVED:
				GRAPHITE_LOG_FATAL("Present failed: Device Removed.");
				break;
			default:
				GRAPHITE_LOG_FATAL("Present failed: Unknown Error.");
				break;
			}
		}

		MoveToNextFrame();
		ProcessDeferrals(m_CurrentBackBuffer);
	}

	void D3D12GraphicsContext::ResizeBackBuffer(uint32_t width, uint32_t height)
	{
		GRAPHITE_ASSERT(width && height, "Invalid back buffer size!");

		GRAPHITE_LOG_INFO("Resizing back buffers to ({}x{})", width, height);
		m_BackBufferWidth = width;
		m_BackBufferHeight = height;

		// Free previous swap chain render targets and descriptors
		m_BackBufferRTVs.Free();
		for (auto& buffer : m_BackBuffers)
		{
			buffer.ReleaseAndGetAddressOf();
		}

		// Wait until idle
		WaitForGPUIdle();

		ProcessAllDeferrals();

		// Resize
		const auto flags = m_TearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
		DX_THROW_IF_FAIL(m_SwapChain->ResizeBuffers(s_BackBufferCount, m_BackBufferWidth, m_BackBufferHeight, m_NativeBackBufferFormat, flags));

		m_CurrentBackBuffer = m_SwapChain->GetCurrentBackBufferIndex();

		BOOL fullscreenState;
		DX_THROW_IF_FAIL(m_SwapChain->GetFullscreenState(&fullscreenState, nullptr));
		m_WindowedMode = !fullscreenState;

		// Recreate resources for back buffers
		CreateBackBufferRTVs();

		m_FrameResources.at(m_CurrentBackBuffer).SetFence(m_DirectQueue->Signal());

		// Wait for GPU to finish its work before continuing
		m_DirectQueue->WaitForIdleCPUBlocking();
	}

	void D3D12GraphicsContext::DeferResourceRelease(const ComPtr<IUnknown>& resource)
	{
		m_FrameResources.at(m_CurrentBackBuffer).DeferRelease(resource);
	}

	void D3D12GraphicsContext::WaitForGPUIdle() const
	{
		m_DirectQueue->WaitForIdleCPUBlocking();
		m_ComputeQueue->WaitForIdleCPUBlocking();
		m_CopyQueue->WaitForIdleCPUBlocking();
	}

	std::unique_ptr<GraphicsPipeline> D3D12GraphicsContext::CreateGraphicsPipeline(const GraphicsPipelineDescription& description)
	{
		return std::make_unique<D3D12GraphicsPipeline>(*this, description);
	}

	DescriptorAllocation D3D12GraphicsContext::AllocateStaticDescriptors(uint32_t count)
	{
		return m_StaticDescriptorAllocator.Allocate(count);
	}

	DescriptorAllocation D3D12GraphicsContext::AllocateDynamicDescriptors(uint32_t count)
	{
		return m_DynamicDescriptorAllocators.at(m_CurrentBackBuffer).Allocate(count);
	}

	DescriptorAllocation D3D12GraphicsContext::AllocateStagingDescriptors(uint32_t count)
	{
		return m_StagingDescriptorAllocator.Allocate(count);
	}


	void D3D12GraphicsContext::CreateConstantBufferView(GPUVirtualAddress bufferAddress, uint32_t bufferSize, CPUDescriptorHandle destDescriptor)
	{
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbv{
			.BufferLocation = bufferAddress,
			.SizeInBytes = bufferSize
		};
		m_Device->CreateConstantBufferView(&cbv, GraphiteCPUDescriptorToD3D12Descriptor(destDescriptor));
	}

	void D3D12GraphicsContext::CopyDescriptors(CPUDescriptorHandle source, CPUDescriptorHandle destination, uint32_t descriptorCount, DescriptorHeapType type)
	{
		m_Device->CopyDescriptorsSimple(
			descriptorCount,
			GraphiteCPUDescriptorToD3D12Descriptor(destination),
			GraphiteCPUDescriptorToD3D12Descriptor(source),
			GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(type)
			);
	}


	/*
	 * Graphics Context Initialization
	 */

	void D3D12GraphicsContext::CreateAdapter()
	{
		UINT dxgiFactoryFlags = 0;

#ifdef GRAPHITE_DEBUG
		{
			// Enable debug layer
			// TODO: Acquire latest debug layer available for improved debugging
			ComPtr<ID3D12Debug> debugController;
			HRESULT result = D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()));
			if (SUCCEEDED(result))
			{
				debugController->EnableDebugLayer();
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

				GRAPHITE_LOG_INFO("Enabled D3D12 debug layer");
			}
			else
			{
				GRAPHITE_LOG_WARN("Failed to D3D12 enable debug layer!");
				DX_LOG_HR_ERROR(result);
			}
		}
#endif

		// Create factory
		DX_THROW_IF_FAIL(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_Factory.ReleaseAndGetAddressOf())));

		// Check tearing support
		{
			BOOL allowTearing = FALSE;
			const HRESULT result = m_Factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
			m_TearingSupport = SUCCEEDED(result) && allowTearing;
		}

		// Select adapter
		for (UINT adapterIndex = 0;
			SUCCEEDED(m_Factory->EnumAdapterByGpuPreference(
				adapterIndex,
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
				IID_PPV_ARGS(m_Adapter.ReleaseAndGetAddressOf())));
			++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			m_Adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			// Test to make sure this adapter supports D3D12 - but don't actually create the device yet
			if (SUCCEEDED(
				D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)
			))
			{
#ifdef GRAPHITE_DEBUG
				std::wstring deviceInfo;
				deviceInfo += L"Selected Adapter:\n\t";
				deviceInfo += desc.Description;
				deviceInfo += L"\n\tAvailable Dedicated Video Memory: ";
				deviceInfo += std::to_wstring(desc.DedicatedVideoMemory / 1000000000.f);
				deviceInfo += L" GB";
				GRAPHITE_LOG_INFO(deviceInfo.c_str());
#endif

				// Satisfactory adapter has been found
				break;
			}
		}

		// TODO: This shouldn't be a debug only assert - this should terminate the application in release
		GRAPHITE_ASSERT(m_Adapter, "Failed to find any adapter!");
	}

	void D3D12GraphicsContext::CreateDevice()
	{
		DX_THROW_IF_FAIL(D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_Device.ReleaseAndGetAddressOf())));

#ifdef GRAPHITE_DEBUG
		// Create info queue to receive debug callback
		// Debug layer must be enabled for this to succeed
		HRESULT result = m_Device->QueryInterface(IID_PPV_ARGS(m_InfoQueue.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(result))
		{
			// TODO: Setup message callback

			DX_THROW_IF_FAIL(m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true));
			DX_THROW_IF_FAIL(m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true));
			DX_THROW_IF_FAIL(m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true));
		}
		else
		{
			DX_LOG_HR_ERROR(result);
			GRAPHITE_LOG_WARN("Failed to create Info Queue. D3D12 messages and TDR debug information will not be received.");
		}
#endif
	}

	void D3D12GraphicsContext::CreateCommandQueues()
	{
		m_DirectQueue = std::make_unique<D3D12CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT, L"Direct Command Queue");
		m_ComputeQueue = std::make_unique<D3D12CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_COMPUTE, L"Compute Command Queue");
		m_CopyQueue = std::make_unique<D3D12CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_COPY, L"Copy Command Queue");
	}


	void D3D12GraphicsContext::CreateSwapChain(HWND windowHandle)
	{
		// Describe and create the swap chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = s_BackBufferCount;
		swapChainDesc.Width = m_BackBufferWidth;
		swapChainDesc.Height = m_BackBufferHeight;
		swapChainDesc.Format = m_NativeBackBufferFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Flags = m_TearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		ComPtr<IDXGISwapChain1> swapChain;
		DX_THROW_IF_FAIL(m_Factory->CreateSwapChainForHwnd(
			m_DirectQueue->GetRHIQueue(), // Swap chain needs the queue so that it can force a flush on it.
			windowHandle,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		));

		// Currently no support for fullscreen transitions
		DX_THROW_IF_FAIL(m_Factory->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER));

		DX_THROW_IF_FAIL(swapChain.As(&m_SwapChain));
		m_CurrentBackBuffer = m_SwapChain->GetCurrentBackBufferIndex();

		BOOL fullscreenState;
		DX_THROW_IF_FAIL(m_SwapChain->GetFullscreenState(&fullscreenState, nullptr));
		m_WindowedMode = !fullscreenState;
	}

	void D3D12GraphicsContext::CreateDescriptorHeaps()
	{
		m_ResourceHeap = std::make_unique<D3D12DescriptorHeap>(m_Device.Get(), GraphiteDescriptorHeap_Resource, 1024, false, L"Resource Descriptor Heap");
		m_SamplerHeap = std::make_unique<D3D12DescriptorHeap>(m_Device.Get(), GraphiteDescriptorHeap_Sampler, 16, false, L"Sampler Descriptor Heap");

		m_StagingHeap = std::make_unique<D3D12DescriptorHeap>(m_Device.Get(), GraphiteDescriptorHeap_Resource, 1024, true, L"Staging Descriptor Heap");

		m_DSVHeap = std::make_unique<D3D12DescriptorHeap>(m_Device.Get(), GraphiteDescriptorHeap_DSV, 64, true, L"DSV Descriptor Heap");
		m_RTVHeap = std::make_unique<D3D12DescriptorHeap>(m_Device.Get(), GraphiteDescriptorHeap_RTV, 64, true, L"RTV Descriptor Heap");

		// Create allocators for the resource heap
		uint32_t descriptorsPerHeap = m_ResourceHeap->GetCapacity() / 4;
		uint32_t allocatorHeapOffset = 0;

		m_StaticDescriptorAllocator = StaticDescriptorAllocator(m_ResourceHeap.get(), allocatorHeapOffset, descriptorsPerHeap);
		allocatorHeapOffset += descriptorsPerHeap;

		for (uint32_t i = 0; i < s_BackBufferCount; i++)
		{
			m_DynamicDescriptorAllocators.at(i) = DynamicDescriptorAllocator(m_ResourceHeap.get(), allocatorHeapOffset, descriptorsPerHeap, i);
			allocatorHeapOffset += descriptorsPerHeap;
		}

		m_StagingDescriptorAllocator = StaticDescriptorAllocator(m_StagingHeap.get(), 0, m_StagingHeap->GetCapacity());
		m_RTVAllocator = StaticDescriptorAllocator(m_RTVHeap.get(), 0, m_RTVHeap->GetCapacity());
	}

	void D3D12GraphicsContext::CreateBackBufferRTVs()
	{
		m_BackBufferRTVs = m_RTVAllocator.Allocate(s_BackBufferCount);
		GRAPHITE_ASSERT(m_BackBufferRTVs.IsValid(), "RTV descriptor alloc failed");

		// Create an RTV for each frame
		for (UINT n = 0; n < s_BackBufferCount; n++)
		{
			DX_THROW_IF_FAIL(m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_BackBuffers.at(n))));
			D3D12_CPU_DESCRIPTOR_HANDLE rtv = GraphiteCPUDescriptorToD3D12Descriptor(m_BackBufferRTVs.GetCPUHandle(n));
			m_Device->CreateRenderTargetView(m_BackBuffers.at(n).Get(), nullptr, rtv);

#ifdef GRAPHITE_DEBUG
			std::wstring name(L"Back Buffer ");
			name += std::to_wstring(n);
			DX_THROW_IF_FAIL(m_BackBuffers.at(n)->SetName(name.c_str()));
#endif
		}
	}

	void D3D12GraphicsContext::CreateFrameResources(uint32_t allocatorPoolSize)
	{
		for (uint32_t i = 0; i < s_BackBufferCount; i++)
		{
			m_FrameResources.at(i).Init(m_Device.Get(), i, allocatorPoolSize);
		}
	}

	void D3D12GraphicsContext::CreateRecordingContexts(uint32_t recordingContextCount)
	{
		m_RecordingContexts.reserve(recordingContextCount);
		m_PendingCommandLists.reserve(recordingContextCount);

		for (uint32_t i = 0; i < recordingContextCount; i++)
		{
			auto allocator = m_FrameResources.at(m_CurrentBackBuffer).GetNextCommandAllocator();
			D3D12CommandRecordingContext context{ m_Device.Get(), allocator, D3D12_COMMAND_LIST_TYPE_DIRECT };
			// Contexts are open by default - but should be closed before the main loop begins
			context.Close();

			m_RecordingContexts.emplace_back(std::move(context));
		}
	}


	void D3D12GraphicsContext::FlushPendingCommandLists()
	{
		UINT64 fenceValue = m_DirectQueue->ExecuteCommandLists(static_cast<UINT>(m_PendingCommandLists.size()), m_PendingCommandLists.data());
		m_FrameResources.at(m_CurrentBackBuffer).SetFence(fenceValue);

		m_PendingCommandLists.clear();
	}

	void D3D12GraphicsContext::MoveToNextFrame()
	{
		m_CurrentBackBuffer = m_SwapChain->GetCurrentBackBufferIndex();

		// If they are still being processed by the GPU, then wait until they are ready
		m_DirectQueue->WaitForFenceCPUBlocking(m_FrameResources.at(m_CurrentBackBuffer).GetFenceValue());
	}

	void D3D12GraphicsContext::ProcessDeferrals(uint32_t frameIndex)
	{
		m_FrameResources[frameIndex].ProcessDeferrals();

		// Propagate deferred free to all descriptor allocators
		m_StaticDescriptorAllocator.ReleasePendingFrees(frameIndex);
		for (auto& alloc : m_DynamicDescriptorAllocators)
		{
			alloc.ReleasePendingFrees(frameIndex);
		}

		m_StagingDescriptorAllocator.ReleasePendingFrees(frameIndex);
		m_RTVAllocator.ReleasePendingFrees(frameIndex);
	}

	void D3D12GraphicsContext::ProcessAllDeferrals()
	{
		for (UINT n = 0; n < s_BackBufferCount; ++n)
			ProcessDeferrals(n);
	}
}
