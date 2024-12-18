#pragma once
#include "CommandQueue.h"
#include "FrameResources.h"
#include "Graphite/Core/Core.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{
	struct GraphiteGraphicsContextDesc
	{
		// The window that the graphics context is being created for
		HWND WindowHandle;

		// Back buffer description
		uint32_t BackBufferWidth;
		uint32_t BackBufferHeight;
		DXGI_FORMAT BackBufferFormat;
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

		void Present();

		void ClearBackBuffer();

		void ResizeBackBuffer(uint32_t width, uint32_t height);

		void WaitForGPUIdle() const;

	public:
		// Getters
		inline static constexpr uint32_t GetBackBufferCount() { return s_BackBufferCount; }
		inline uint32_t GetCurrentBackBuffer() const { return m_CurrentBackBuffer; }

		inline uint32_t GetBackBufferWidth() const { return m_BackBufferWidth; }
		inline uint32_t GetBackBufferHeight() const { return m_BackBufferHeight; }

		// VSync
		inline bool GetVSync() const { return m_VSync; }
		inline void SetVSync(bool vsync) { m_VSync = vsync; }

	private:
		// Initialization
		void CreateAdapter();
		void CreateDevice();
		void CreateCommandQueues();
		void CreateSwapChain(HWND windowHandle);
		void CreateCommandAllocator();
		void CreateCommandList();
		void CreateFrameResources();

	private:
		void MoveToNextFrame();

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

		ComPtr<ID3D12CommandAllocator> m_DirectCommandAllocator;	// Used for non-frame-specific allocations (startup, resize swap chain, etc)
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		// Frame resources
		std::array<FrameResources, s_BackBufferCount> m_FrameResources;

		// State
		uint32_t m_CurrentBackBuffer = 0;

		bool m_VSync = true;
		bool m_WindowedMode = true;
	};

}
