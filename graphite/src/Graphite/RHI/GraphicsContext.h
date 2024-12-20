#pragma once

#include "Graphite/Core/Core.h"

#include "CommandQueue.h"
#include "DescriptorHeap.h"
#include "FrameResources.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{
	class CommandRecordingContext;


	struct GraphiteGraphicsContextDesc
	{
		// The window that the graphics context is being created for
		HWND WindowHandle;

		// Back buffer description
		uint32_t BackBufferWidth;
		uint32_t BackBufferHeight;
		DXGI_FORMAT BackBufferFormat;

		// For multithreaded command recording
		uint32_t MaxRecordingContextsPerFrame;
	};

	class GraphicsContext
	{
	public:
		GraphicsContext(const GraphiteGraphicsContextDesc& contextDesc);
		~GraphicsContext();

		DELETE_COPY(GraphicsContext);
		DEFAULT_MOVE(GraphicsContext);

		// Application API
		void BeginFrame();
		void EndFrame();

		// Multiple recording contexts can be active at a time
		// This is to facilitate multithreaded command recording
		CommandRecordingContext* AcquireRecordingContext();
		void CloseRecordingContext(CommandRecordingContext* recordingContext);

		void Present();

		void ResizeBackBuffer(uint32_t width, uint32_t height);

		void DeferResourceRelease(const ComPtr<IUnknown>& resource);
		void WaitForGPUIdle() const;

	public:
		// Getters
		inline static constexpr uint32_t GetBackBufferCount() { return s_BackBufferCount; }
		inline uint32_t GetCurrentBackBuffer() const { return m_CurrentBackBuffer; }

		inline uint32_t GetBackBufferWidth() const { return m_BackBufferWidth; }
		inline uint32_t GetBackBufferHeight() const { return m_BackBufferHeight; }

		inline D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferRenderTargetView() const { return m_BackBufferRTVs.GetCPUHandle(m_CurrentBackBuffer); }

		// VSync
		inline bool GetVSync() const { return m_VSync; }
		inline void SetVSync(bool vsync) { m_VSync = vsync; }

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
		inline static constexpr uint32_t s_BackBufferCount = 3;

		uint32_t m_BackBufferWidth;
		uint32_t m_BackBufferHeight;

		DXGI_FORMAT m_BackBufferFormat;

		// API resources
		ComPtr<IDXGIFactory6> m_Factory;
		ComPtr<IDXGIAdapter1> m_Adapter;

		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D12InfoQueue1> m_InfoQueue;

		ComPtr<IDXGISwapChain3> m_SwapChain;

		// Command queues
		std::unique_ptr<CommandQueue> m_DirectQueue;
		std::unique_ptr<CommandQueue> m_ComputeQueue;
		std::unique_ptr<CommandQueue> m_CopyQueue;

		// Frame resources
		std::array<FrameResources, s_BackBufferCount> m_FrameResources;

		std::vector<CommandRecordingContext> m_RecordingContexts;
		std::mutex m_RecordingContextAcquisitionMutex;

		inline static constexpr int32_t s_InternalCommandRecordingContexts = 2;	// Some recording contexts are reserved for graphics context use
																				// One for beginning and one for ending the frame
		size_t m_RecordingContextsUsedThisFrame = 0;	// The number of recording contexts that were acquired at any point during the frame
		size_t m_OpenRecordingContexts = 0;				// The number of recording contexts currently active, that should be closed before the end of the frame

		std::vector<ID3D12CommandList*> m_PendingCommandLists;

		// Descriptor heaps
		DescriptorHeap m_ResourceHeap;
		DescriptorHeap m_SamplerHeap;

		DescriptorHeap m_DSVHeap;
		DescriptorHeap m_RTVHeap;

		// Back buffer resources and descriptors
		std::array<ComPtr<ID3D12Resource>, s_BackBufferCount> m_BackBuffers;
		DescriptorAllocation m_BackBufferRTVs;

		// State
		uint32_t m_CurrentBackBuffer = 0;

		bool m_VSync = true;
		bool m_WindowedMode = true;
	};

}
