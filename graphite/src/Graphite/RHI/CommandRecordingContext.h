#pragma once

#include "Graphite/Core/Core.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{

	class CommandRecordingContext
	{
		// Recording contexts should only be created by the graphics context
		friend class GraphicsContext;
		CommandRecordingContext(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name = nullptr);
	public:
		~CommandRecordingContext() = default;

		DELETE_COPY(CommandRecordingContext);
		DEFAULT_MOVE(CommandRecordingContext);

		// Command recording API
		void SetDescriptorHeaps(ID3D12DescriptorHeap* resourceHeap, ID3D12DescriptorHeap* samplerHeap) const;

		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtv, float* clearColor) const;
		void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth, uint8_t stencil) const;

		void SetRenderTargets(std::span<const D3D12_CPU_DESCRIPTOR_HANDLE> rtvRange, std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> dsv) const;

		void SetPipelineState(ID3D12PipelineState* pipelineState) const;

		// Graphics
		void SetGraphicsRootSignature(ID3D12RootSignature* rootSignature) const;

		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const;
		void SetViewports(std::span<const D3D12_VIEWPORT> viewports) const;
		void SetScissorRects(std::span<const D3D12_RECT> rects) const;

		void SetVertexBuffers(uint32_t startSlot, std::span<const D3D12_VERTEX_BUFFER_VIEW> vertexBuffers) const;
		void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBuffer) const;

		void DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertex, uint32_t startInstance) const;
		void DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const;

	public:
		// Getters

		ID3D12GraphicsCommandList* GetCommandList() const { return m_CommandList.Get(); }

		bool IsClosed() const { return m_IsClosed; }

	private:
		// Called by the graphics context
		void Reset(ID3D12CommandAllocator* allocator);
		void Close();

	private:
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		bool m_IsClosed = false;
	};
}
