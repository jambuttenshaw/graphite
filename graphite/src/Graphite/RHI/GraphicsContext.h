#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/Core/Assert.h"

#include "RHITypes.h"
#include "Descriptors.h"


namespace Graphite
{
	class CommandRecordingContext;
	class DescriptorHeap;

	class GraphicsPipeline;
	struct GraphicsPipelineDescription;


	struct GraphicsContextDesc
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
		GRAPHITE_API static GraphicsContext* Get();
	protected:
		GRAPHITE_API GraphicsContext(const GraphicsContextDesc& contextDesc);
	public:
		GRAPHITE_API virtual ~GraphicsContext() = default;

		GRAPHITE_API_DELETE_COPY(GraphicsContext);
		GRAPHITE_API_DEFAULT_MOVE(GraphicsContext);

		// These are called internally to Graphite
		GRAPHITE_API virtual void BeginFrame() = 0;
		GRAPHITE_API virtual void EndFrame() = 0;

		// Application API

		// Passes are ways of grouping command submission
		// Populated command lists will be submitted at the end of a pass
		// All open recording contexts should be closed before ending a pass
		// Work submitted within the same pass can be executed concurrently on the GPU as decided by the driver
		// Therefore all command recording contexts within a pass must be independent
		GRAPHITE_API virtual void BeginPass() = 0;
		GRAPHITE_API virtual void EndPass() = 0;

		// Multiple recording contexts can be active at a time
		// This is to facilitate multithreaded command recording
		GRAPHITE_API virtual CommandRecordingContext* AcquireRecordingContext() = 0;
		GRAPHITE_API virtual void CloseRecordingContext(CommandRecordingContext* recordingContext) = 0;

		GRAPHITE_API virtual void Present() = 0;

		GRAPHITE_API virtual void ResizeBackBuffer(uint32_t width, uint32_t height) = 0;

		GRAPHITE_API virtual void WaitForGPUIdle() const = 0;

	public:
		// Getters
		GRAPHITE_API inline static constexpr uint32_t GetBackBufferCount() { return s_BackBufferCount; }
		GRAPHITE_API inline uint32_t GetCurrentBackBuffer() const { return m_CurrentBackBuffer; }

		GRAPHITE_API inline uint32_t GetBackBufferWidth() const { return m_BackBufferWidth; }
		GRAPHITE_API inline uint32_t GetBackBufferHeight() const { return m_BackBufferHeight; }

		GRAPHITE_API virtual CPUDescriptorHandle GetBackBufferRenderTargetView() const = 0;

		// Object creation

		GRAPHITE_API virtual std::unique_ptr<GraphicsPipeline> CreateGraphicsPipeline(const GraphicsPipelineDescription& description) = 0;

		// Resource management

		GRAPHITE_API virtual DescriptorAllocation AllocateStaticDescriptors(uint32_t count) = 0;
		GRAPHITE_API virtual DescriptorAllocation AllocateDynamicDescriptors(uint32_t count) = 0;
		// Staging descriptors are CPU only and therefore GPU synchronization doesn't have to be considered
		GRAPHITE_API virtual DescriptorAllocation AllocateStagingDescriptors(uint32_t count) = 0;

		GRAPHITE_API virtual void CreateConstantBufferView(GPUVirtualAddress bufferAddress, uint32_t bufferSize, CPUDescriptorHandle destDescriptor) = 0;

		GRAPHITE_API virtual void CopyDescriptors(CPUDescriptorHandle source, CPUDescriptorHandle destination, uint32_t descriptorCount, DescriptorHeapType type) = 0;

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
