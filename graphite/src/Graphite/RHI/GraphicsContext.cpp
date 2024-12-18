#include "pch.h"
#include "GraphicsContext.h"

#include "RHIExceptions.h"
#include "Graphite/Core/Assert.h"


namespace Graphite
{
	GraphicsContext::GraphicsContext(const GraphiteGraphicsContextDesc& contextDesc)
		: m_BackBufferWidth(contextDesc.BackBufferWidth)
		, m_BackBufferHeight(contextDesc.BackBufferHeight)
		, m_BackBufferFormat(contextDesc.BackBufferFormat)
	{
		static bool s_GraphicsContextExists = false;
		GRAPHITE_ASSERT(!s_GraphicsContextExists, "Only one graphics context may exist!");
		s_GraphicsContextExists = true;

		GRAPHITE_LOG_INFO("Creating D3D12 Graphics Context...");

		// Initialization
		CreateAdapter();
		CreateDevice();
		CreateCommandQueues();
		CreateSwapChain(contextDesc.WindowHandle);
		CreateCommandAllocator();
		CreateCommandList();
		CreateFrameResources();

		// Close the command list and execute it to begin the initial GPU set up
		// The main loop will expect the command list to be closed
		DX_THROW_IF_FAIL(m_CommandList->Close());
		ID3D12CommandList* ppCommandLists[] = { m_CommandList.Get() };
		const auto fenceValue = m_DirectQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		m_FrameResources.at(m_CurrentBackBuffer).SetFence(fenceValue);

		// Wait for any GPU work executed on startup to finish before continuing
		m_DirectQueue->WaitForIdleCPUBlocking();

		GRAPHITE_LOG_INFO("D3D12 Graphics Context created successfully.");
	}

	GraphicsContext::~GraphicsContext()
	{
		// Wait for all work on GPU to complete before releasing resources
		WaitForGPUIdle();
	}


	void GraphicsContext::BeginFrame()
	{
		m_FrameResources.at(m_CurrentBackBuffer).ResetAllocator();
		// Command lists must be reset after ExecuteCommandList() is called and before it is repopulated
		DX_THROW_IF_FAIL(m_CommandList->Reset(m_FrameResources.at(m_CurrentBackBuffer).GetCommandAllocator(), nullptr));
	}

	void GraphicsContext::EndFrame()
	{
		DX_THROW_IF_FAIL(m_CommandList->Close());

		// Execute the command list
		ID3D12CommandList* ppCommandLists[] = { m_CommandList.Get() };
		const auto fenceValue = m_DirectQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		m_FrameResources.at(m_CurrentBackBuffer).SetFence(fenceValue);
	}

	void GraphicsContext::Present()
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
	}

	void GraphicsContext::ClearBackBuffer()
	{
		
	}

	void GraphicsContext::ResizeBackBuffer(uint32_t width, uint32_t height)
	{
		GRAPHITE_ASSERT(width && height, "Invalid back buffer size!");

		GRAPHITE_LOG_INFO("Resizing back buffers to ({}x{})", width, height);
		m_BackBufferWidth = width;
		m_BackBufferHeight = height;

		// Free previous swap chain render targets and descriptors

		// Wait until idle
		WaitForGPUIdle();
		DX_THROW_IF_FAIL(m_CommandList->Reset(m_DirectCommandAllocator.Get(), nullptr));

		// Resize
		const auto flags = m_TearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
		DX_THROW_IF_FAIL(m_SwapChain->ResizeBuffers(s_BackBufferCount, m_BackBufferWidth, m_BackBufferHeight, m_BackBufferFormat, flags));

		m_CurrentBackBuffer = m_SwapChain->GetCurrentBackBufferIndex();

		BOOL fullscreenState;
		DX_THROW_IF_FAIL(m_SwapChain->GetFullscreenState(&fullscreenState, nullptr));
		m_WindowedMode = !fullscreenState;

		// Submit required work to re-init buffers
		DX_THROW_IF_FAIL(m_CommandList->Close());
		ID3D12CommandList* ppCommandLists[] = { m_CommandList.Get() };
		const auto fenceValue = m_DirectQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		m_FrameResources.at(m_CurrentBackBuffer).SetFence(fenceValue);

		// Wait for GPU to finish its work before continuing
		m_DirectQueue->WaitForIdleCPUBlocking();
	}

	void GraphicsContext::WaitForGPUIdle() const
	{
		m_DirectQueue->WaitForIdleCPUBlocking();
		m_ComputeQueue->WaitForIdleCPUBlocking();
		m_CopyQueue->WaitForIdleCPUBlocking();
	}


	/*
	 * Graphics Context Initialization
	 */

	void GraphicsContext::CreateAdapter()
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

	void GraphicsContext::CreateDevice()
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

	void GraphicsContext::CreateCommandQueues()
	{
		m_DirectQueue = std::make_unique<CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT, L"Direct Command Queue");
		m_ComputeQueue = std::make_unique<CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_COMPUTE, L"Compute Command Queue");
		m_CopyQueue = std::make_unique<CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_COPY, L"Copy Command Queue");
	}


	void GraphicsContext::CreateSwapChain(HWND windowHandle)
	{
		// Describe and create the swap chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = s_BackBufferCount;
		swapChainDesc.Width = m_BackBufferWidth;
		swapChainDesc.Height = m_BackBufferHeight;
		swapChainDesc.Format = m_BackBufferFormat;
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

	void GraphicsContext::CreateCommandAllocator()
	{
		DX_THROW_IF_FAIL(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_DirectCommandAllocator)));
		m_DirectCommandAllocator->SetName(L"Graphics Context Direct Command Allocator");
	}

	void GraphicsContext::CreateCommandList()
	{
		// Create the command list
		DX_THROW_IF_FAIL(m_Device->CreateCommandList(0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_DirectCommandAllocator.Get(), nullptr,
			IID_PPV_ARGS(&m_CommandList)));
		m_CommandList->SetName(L"Graphics Context Command List");
	}

	void GraphicsContext::CreateFrameResources()
	{
		for (uint32_t i = 0; i < s_BackBufferCount; i++)
		{
			m_FrameResources.at(i).Init(m_Device.Get(), i);
		}
	}



	void GraphicsContext::MoveToNextFrame()
	{
		m_CurrentBackBuffer = m_SwapChain->GetCurrentBackBufferIndex();

		// If they are still being processed by the GPU, then wait until they are ready
		m_DirectQueue->WaitForFenceCPUBlocking(m_FrameResources.at(m_CurrentBackBuffer).GetFenceValue());
	}


}
