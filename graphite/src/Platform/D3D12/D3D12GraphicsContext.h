#pragma once

#include "Graphite/Core/Core.h"

#include "Graphite/RHI/GraphicsContext.h"

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
	protected:
		friend GraphicsContext;
		D3D12GraphicsContext(const GraphiteGraphicsContextDesc& contextDesc);
	public:
		GRAPHITE_API virtual ~D3D12GraphicsContext();

		GRAPHITE_API_DELETE_COPY(D3D12GraphicsContext);
		GRAPHITE_API_DEFAULT_MOVE(D3D12GraphicsContext);

		// Application API
		GRAPHITE_API virtual void BeginFrame() override;
		GRAPHITE_API virtual void EndFrame() override;

		// Multiple recording contexts can be active at a time
		// This is to facilitate multithreaded command recording
		GRAPHITE_API virtual CommandRecordingContext* AcquireRecordingContext() override;
		GRAPHITE_API virtual void CloseRecordingContext(CommandRecordingContext* recordingContext) override;

		GRAPHITE_API virtual void Present() override;

		GRAPHITE_API virtual void ResizeBackBuffer(uint32_t width, uint32_t height) override;

		GRAPHITE_API virtual void DeferResourceRelease(const ComPtr<IUnknown>& resource);
		GRAPHITE_API virtual void WaitForGPUIdle() const override;

	public:

		GRAPHITE_API inline DXGI_FORMAT GetNativeBackBufferFormat() const { return m_NativeBackBufferFormat; }
		GRAPHITE_API inline D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferRenderTargetView() const { return m_BackBufferRTVs.GetCPUHandle(m_CurrentBackBuffer); }

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

		// Back buffer resources and descriptors
		std::array<ComPtr<ID3D12Resource>, s_BackBufferCount> m_BackBuffers;
		D3D12DescriptorAllocation m_BackBufferRTVs;
	};
};
