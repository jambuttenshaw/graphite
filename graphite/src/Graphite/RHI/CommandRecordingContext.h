#pragma once

#include "Graphite/Core/Core.h"
#include "RHITypes.h"


namespace Graphite
{
	class DescriptorHeap;
	class GraphicsPipeline;

	class ResourceViewList;

	class VertexBuffer;
	struct IndexBufferView;


	class CommandRecordingContext
	{
	protected:
		// Recording contexts should only be created by the graphics context
		GRAPHITE_API CommandRecordingContext() = default;
	public:
		GRAPHITE_API virtual ~CommandRecordingContext() = default;

		GRAPHITE_API DELETE_COPY(CommandRecordingContext);
		GRAPHITE_API DEFAULT_MOVE(CommandRecordingContext);

		// Command recording API
		GRAPHITE_API virtual void SetDescriptorHeaps(DescriptorHeap* resourceHeap, DescriptorHeap* samplerHeap) const = 0;

		GRAPHITE_API virtual void ClearRenderTargetView(CPUDescriptorHandle rtv, const glm::vec4& clearColor) const = 0;
		GRAPHITE_API virtual void ClearDepthStencilView(CPUDescriptorHandle dsv, float depth, uint8_t stencil) const = 0;

		GRAPHITE_API virtual void SetRenderTargets(uint32_t rtvCount, CPUDescriptorHandle rtvRange, std::optional<CPUDescriptorHandle> dsv) const = 0;

		GRAPHITE_API virtual void SetGraphicsPipelineState(const GraphicsPipeline& pipelineState) const = 0;

		GRAPHITE_API virtual void SetGraphicsPipelineResources(const ResourceViewList& resourceViewList) const = 0;

		GRAPHITE_API virtual void SetPrimitiveTopology(GraphiteTopology topology) const = 0;

		GRAPHITE_API virtual void SetViewports(std::span<const Viewport> viewports) const = 0;
		GRAPHITE_API virtual void SetScissorRects(std::span<const Rectangle> rects) const = 0;

		GRAPHITE_API virtual void SetVertexBuffers(uint32_t startSlot, const VertexBuffer& vertexBuffer) const = 0;
		GRAPHITE_API virtual void SetIndexBuffer(const IndexBufferView& indexBuffer) const = 0;

		GRAPHITE_API virtual void DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertex, uint32_t startInstance) const = 0;
		GRAPHITE_API virtual void DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const = 0;

	public:
		// Getters
		GRAPHITE_API bool IsClosed() const { return m_IsClosed; }

	protected:

		// Called by the graphics context
		// allocator is a pointer to a native command allocator object
		GRAPHITE_API virtual void Reset(void* allocator) = 0;
		GRAPHITE_API virtual void Close() = 0;

	protected:
		bool m_IsClosed = false;
	};
}
