#pragma once

#include "Graphite/Core/Core.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{
	class DescriptorHeap;


	class CommandRecordingContext
	{
	protected:
		// Recording contexts should only be created by the graphics context
		CommandRecordingContext() = default;
	public:
		virtual ~CommandRecordingContext() = default;

		DELETE_COPY(CommandRecordingContext);
		DEFAULT_MOVE(CommandRecordingContext);

		// Command recording API
		virtual void SetDescriptorHeaps(DescriptorHeap* resourceHeap, DescriptorHeap* samplerHeap) const = 0;

		virtual void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtv, float* clearColor) const = 0;
		virtual void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth, uint8_t stencil) const = 0;

		virtual void SetRenderTargets(std::span<const D3D12_CPU_DESCRIPTOR_HANDLE> rtvRange, std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> dsv) const = 0;

		virtual void SetPipelineState(ID3D12PipelineState* pipelineState) const = 0;
		virtual void SetGraphicsRootSignature(ID3D12RootSignature* rootSignature) const = 0;

		virtual void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const = 0;

		virtual void SetViewports(std::span<const D3D12_VIEWPORT> viewports) const = 0;
		virtual void SetScissorRects(std::span<const D3D12_RECT> rects) const = 0;

		virtual void SetVertexBuffers(uint32_t startSlot, std::span<const D3D12_VERTEX_BUFFER_VIEW> vertexBuffers) const = 0;
		virtual void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBuffer) const = 0;

		virtual void DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertex, uint32_t startInstance) const = 0;
		virtual void DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const = 0;

	public:
		// Getters
		bool IsClosed() const { return m_IsClosed; }

	protected:

		// Called by the graphics context
		virtual void Reset(ID3D12CommandAllocator* allocator) = 0;
		virtual void Close() = 0;

	protected:
		bool m_IsClosed = false;
	};
}
