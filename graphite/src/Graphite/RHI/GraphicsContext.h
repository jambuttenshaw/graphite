#pragma once

#include "Graphite/Core/Core.h"
#include "RHITypes.h"


namespace Graphite
{
	class CommandRecordingContext;
	class DescriptorHeap;


	struct GraphiteGraphicsContextDesc
	{
		// The window that the graphics context is being created for
		HWND WindowHandle;

		// Back buffer description
		uint32_t BackBufferWidth;
		uint32_t BackBufferHeight;
		GraphiteFormat BackBufferFormat;

		// For multithreaded command recording
		uint32_t MaxRecordingContextsPerFrame;
	};


	class GraphicsContext
	{
	public:
		// Factory
		static std::unique_ptr<GraphicsContext> Create(const GraphiteGraphicsContextDesc& contextDesc);

	protected:
		GraphicsContext(const GraphiteGraphicsContextDesc& contextDesc)
			: m_BackBufferWidth(contextDesc.BackBufferWidth)
			, m_BackBufferHeight(contextDesc.BackBufferHeight)
			, m_BackBufferFormat(contextDesc.BackBufferFormat)
		{}
	public:
		virtual ~GraphicsContext() = default;

		DELETE_COPY(GraphicsContext);
		DEFAULT_MOVE(GraphicsContext);

		// Application API
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		// Multiple recording contexts can be active at a time
		// This is to facilitate multithreaded command recording
		GRAPHITE_API virtual CommandRecordingContext* AcquireRecordingContext() = 0;
		GRAPHITE_API virtual void CloseRecordingContext(CommandRecordingContext* recordingContext) = 0;

		virtual void Present() = 0;

		virtual void ResizeBackBuffer(uint32_t width, uint32_t height) = 0;

		GRAPHITE_API virtual void WaitForGPUIdle() const = 0;

	public:
		// Getters
		GRAPHITE_API inline static constexpr uint32_t GetBackBufferCount() { return s_BackBufferCount; }
		GRAPHITE_API inline uint32_t GetCurrentBackBuffer() const { return m_CurrentBackBuffer; }

		GRAPHITE_API inline uint32_t GetBackBufferWidth() const { return m_BackBufferWidth; }
		GRAPHITE_API inline uint32_t GetBackBufferHeight() const { return m_BackBufferHeight; }

		GRAPHITE_API virtual CPUDescriptorHandle GetBackBufferRenderTargetView() const = 0;

		// Resource management
		GRAPHITE_API virtual DescriptorHeap* GetResourceDescriptorHeap() const = 0;
		GRAPHITE_API virtual DescriptorHeap* GetSamplerDescriptorHeap() const = 0;

		// VSync
		GRAPHITE_API inline bool GetVSync() const { return m_VSync; }
		GRAPHITE_API inline void SetVSync(bool vsync) { m_VSync = vsync; }

	protected:
		// Properties
		inline static constexpr uint32_t s_BackBufferCount = 3;

		uint32_t m_BackBufferWidth;
		uint32_t m_BackBufferHeight;

		GraphiteFormat m_BackBufferFormat;

		// State
		uint32_t m_CurrentBackBuffer = 0;

		bool m_VSync = true;
		bool m_WindowedMode = true;
	};

}
