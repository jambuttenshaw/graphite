#include "graphite_pch.h"
#include "D3D12CommandRecordingContext.h"

#include "D3D12DescriptorHeap.h"
#include "Graphite/Core/Assert.h"
#include "Platform/D3D12/D3D12Exceptions.h"
#include "Pipelines/D3D12GraphicsPipeline.h"


namespace Graphite::D3D12
{
	D3D12CommandRecordingContext::D3D12CommandRecordingContext(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name)
	{
		// Create command list
		DX_THROW_IF_FAIL(device->CreateCommandList(0, type, allocator, nullptr, IID_PPV_ARGS(m_CommandList.ReleaseAndGetAddressOf())));
		m_IsClosed = false;

#ifdef GRAPHITE_DEBUG
		if (name)
		{
			DX_THROW_IF_FAIL(m_CommandList->SetName(name));
		}
#endif
	}

	void D3D12CommandRecordingContext::Reset(ID3D12CommandAllocator* allocator)
	{
		DX_THROW_IF_FAIL(m_CommandList->Reset(allocator, nullptr));
		m_IsClosed = false;
	}

	void D3D12CommandRecordingContext::Close()
	{
		DX_THROW_IF_FAIL(m_CommandList->Close());
		m_IsClosed = true;
	}


	// Command implementations

	void D3D12CommandRecordingContext::SetDescriptorHeaps(DescriptorHeap* resourceHeap, DescriptorHeap* samplerHeap) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		ID3D12DescriptorHeap* heaps[] = {
			dynamic_cast<D3D12DescriptorHeap*>(resourceHeap)->GetHeap(),
			dynamic_cast<D3D12DescriptorHeap*>(samplerHeap)->GetHeap()
		};
		m_CommandList->SetDescriptorHeaps(2, heaps);
	}

	void D3D12CommandRecordingContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView, float* clearColor) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->ClearRenderTargetView(renderTargetView, clearColor, 0, nullptr);
	}

	void D3D12CommandRecordingContext::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth, uint8_t stencil) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
	}

	void D3D12CommandRecordingContext::SetRenderTargets(std::span<const D3D12_CPU_DESCRIPTOR_HANDLE> rtvRange, std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> dsv) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->OMSetRenderTargets(static_cast<UINT>(rtvRange.size()), rtvRange.data(), true, dsv.has_value() ? &dsv.value() : nullptr);
	}

	void D3D12CommandRecordingContext::SetGraphicsPipelineState(const GraphicsPipeline& pipelineState) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		const D3D12GraphicsPipeline& nativePipeline = dynamic_cast<const D3D12GraphicsPipeline&>(pipelineState);
		m_CommandList->SetPipelineState(nativePipeline.GetPipelineState());
		m_CommandList->SetGraphicsRootSignature(nativePipeline.GetRootSignature());
	}

	void D3D12CommandRecordingContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->IASetPrimitiveTopology(topology);
	}

	void D3D12CommandRecordingContext::SetViewports(std::span<const Viewport> viewports) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->RSSetViewports(static_cast<UINT>(viewports.size()), reinterpret_cast<const D3D12_VIEWPORT*>(viewports.data()));
	}

	void D3D12CommandRecordingContext::SetScissorRects(std::span<const Rectangle> rects) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->RSSetScissorRects(static_cast<UINT>(rects.size()), reinterpret_cast<const D3D12_RECT*>(rects.data()));
	}

	void D3D12CommandRecordingContext::SetVertexBuffers(uint32_t startSlot, std::span<const D3D12_VERTEX_BUFFER_VIEW> vertexBuffers) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->IASetVertexBuffers(startSlot, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data());
	}

	void D3D12CommandRecordingContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBuffer) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->IASetIndexBuffer(&indexBuffer);
	}

	void D3D12CommandRecordingContext::DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertex, uint32_t startInstance) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->DrawInstanced(vertexCountPerInstance, instanceCount, startVertex, startInstance);
	}

	void D3D12CommandRecordingContext::DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndex, startVertex, startInstance);
	}
}