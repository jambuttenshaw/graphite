#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/RHI/CommandRecordingContext.h"


using Microsoft::WRL::ComPtr;

namespace Graphite::D3D12
{
	class D3D12CommandRecordingContext : public CommandRecordingContext
	{
		// Recording contexts should only be created by the graphics context4
		friend class D3D12GraphicsContext;
		D3D12CommandRecordingContext(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name = nullptr);
	public:
		virtual ~D3D12CommandRecordingContext() = default;

		DELETE_COPY(D3D12CommandRecordingContext);
		DEFAULT_MOVE(D3D12CommandRecordingContext);

		// Command recording API
		virtual void SetDescriptorHeaps(DescriptorHeap* resourceHeap, DescriptorHeap* samplerHeap) const override;

		virtual void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtv, float* clearColor) const override;
		virtual void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth, uint8_t stencil) const override;

		virtual void SetRenderTargets(std::span<const D3D12_CPU_DESCRIPTOR_HANDLE> rtvRange, std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> dsv) const override;

		virtual void SetGraphicsPipelineState(const GraphicsPipeline& pipelineState) const override;

		virtual void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const override;

		virtual void SetViewports(std::span<const D3D12_VIEWPORT> viewports) const override;
		virtual void SetScissorRects(std::span<const D3D12_RECT> rects) const override;

		virtual void SetVertexBuffers(uint32_t startSlot, std::span<const D3D12_VERTEX_BUFFER_VIEW> vertexBuffers) const override;
		virtual void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBuffer) const override;

		virtual void DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertex, uint32_t startInstance) const override;
		virtual void DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const override;

	public:
		// Getters
		ID3D12GraphicsCommandList* GetCommandList() const { return m_CommandList.Get(); }

	protected:
		// Called by the graphics context
		virtual void Reset(ID3D12CommandAllocator* allocator) override;
		virtual void Close() override;

	private:
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		bool m_IsClosed = false;
	};
}
