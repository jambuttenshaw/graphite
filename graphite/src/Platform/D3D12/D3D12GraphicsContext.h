#pragma once

#include "Graphite/Core/Core.h"

#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/DescriptorAllocators.h"

#include "D3D12FrameResources.h"
#include "D3D12DescriptorHeap.h"


using Microsoft::WRL::ComPtr;


namespace Graphite
{
	class CommandRecordingContext;
}


namespace Graphite::D3D12
{
	class D3D12CommandQueue;
	class D3D12CommandRecordingContext;


	class D3D12GraphicsContext : public GraphicsContext
	{
	public:
		D3D12GraphicsContext(const GraphiteGraphicsContextDesc& contextDesc);
		virtual ~D3D12GraphicsContext();

		DELETE_COPY(D3D12GraphicsContext);
		DEFAULT_MOVE(D3D12GraphicsContext);

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginPass() override;
		virtual void EndPass() override;

		// Multiple recording contexts can be active at a time
		// This is to facilitate multithreaded command recording
		virtual CommandRecordingContext* AcquireRecordingContext() override;
		virtual void CloseRecordingContext(CommandRecordingContext* recordingContext) override;

		virtual void Present() override;

		virtual void ResizeBackBuffer(uint32_t width, uint32_t height) override;

		void DeferResourceRelease(const ComPtr<IUnknown>& resource);
		virtual void WaitForGPUIdle() const override;

	public:
		virtual DescriptorAllocation AllocateStaticDescriptors(uint32_t count) override;
		virtual DescriptorAllocation AllocateDynamicDescriptors(uint32_t count) override;

		virtual void CreateConstantBufferView(GPUVirtualAddress bufferAddress, uint32_t bufferSize, CPUDescriptorHandle destDescriptor) override;

	public:
		inline DXGI_FORMAT GetNativeBackBufferFormat() const { return m_NativeBackBufferFormat; }
		inline CPUDescriptorHandle GetBackBufferRenderTargetView() const override { return m_BackBufferRTVs.GetCPUHandle(m_CurrentBackBuffer); }

		inline IDXGIAdapter* GetAdapter() const { return m_Adapter.Get(); }
		inline ID3D12Device* GetDevice() const { return m_Device.Get(); }

	private:
		// Initialization
		void CreateAdapter();
		void CreateDevice();

		void CreateCommandQueues();
		void CreateSwapChain(HWND windowHandle);

		void CreateDescriptorHeaps();
		void CreateBackBufferRTVs();

		void CreateFrameResources(uint32_t allocatorPoolSize);
		void CreateRecordingContexts(uint32_t recordingContextCount);

	private:

		void FlushPendingCommandLists();

		void MoveToNextFrame();

		void ProcessDeferrals(uint32_t frameIndex);
		// NOTE: this is only safe to do so when GPU is idle
		void ProcessAllDeferrals();

	private:
		// Feature support
		bool m_TearingSupport = false;

		// Properties
		DXGI_FORMAT m_NativeBackBufferFormat = DXGI_FORMAT_UNKNOWN;

		// API resources
		ComPtr<IDXGIFactory6> m_Factory;
		ComPtr<IDXGIAdapter1> m_Adapter;

		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D12InfoQueue1> m_InfoQueue;

		ComPtr<IDXGISwapChain3> m_SwapChain;

		// Command queues
		std::unique_ptr<D3D12CommandQueue> m_DirectQueue;
		std::unique_ptr<D3D12CommandQueue> m_ComputeQueue;
		std::unique_ptr<D3D12CommandQueue> m_CopyQueue;

		// Frame resources
		std::array<D3D12FrameResources, s_BackBufferCount> m_FrameResources;

		std::vector<D3D12CommandRecordingContext> m_RecordingContexts;
		std::mutex m_RecordingContextAcquisitionMutex;

		inline static constexpr int32_t s_InternalCommandRecordingContexts = 2;	// Some recording contexts are reserved for graphics context use
		// One for beginning and one for ending the frame
		size_t m_RecordingContextsUsedThisFrame = 0;	// The number of recording contexts that were acquired at any point during the frame
		size_t m_OpenRecordingContexts = 0;				// The number of recording contexts currently active, that should be closed before the end of the frame

		std::vector<ID3D12CommandList*> m_PendingCommandLists;

		// Descriptor heaps
		std::unique_ptr<D3D12DescriptorHeap> m_ResourceHeap;
		std::unique_ptr<D3D12DescriptorHeap> m_SamplerHeap;

		std::unique_ptr<D3D12DescriptorHeap> m_DSVHeap;
		std::unique_ptr<D3D12DescriptorHeap> m_RTVHeap;

		// Descriptor allocators
		// Resource heap allocators
		StaticDescriptorAllocator m_StaticDescriptorAllocator;
		std::array<DynamicDescriptorAllocator, s_BackBufferCount> m_DynamicDescriptorAllocators;

		StaticDescriptorAllocator m_RTVAllocator;

		// Back buffer resources and descriptors
		std::array<ComPtr<ID3D12Resource>, s_BackBufferCount> m_BackBuffers;
		DescriptorAllocation m_BackBufferRTVs;
	};
};
