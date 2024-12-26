#include "pch.h"
#include "CommandRecordingContext.h"

#include "RHIExceptions.h"
#include "Graphite/Core/Assert.h"


namespace Graphite
{
	CommandRecordingContext::CommandRecordingContext(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name)
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

	void CommandRecordingContext::Reset(ID3D12CommandAllocator* allocator)
	{
		DX_THROW_IF_FAIL(m_CommandList->Reset(allocator, nullptr));
		m_IsClosed = false;
	}

	void CommandRecordingContext::Close()
	{
		DX_THROW_IF_FAIL(m_CommandList->Close());
		m_IsClosed = true;
	}


	// Command implementations

	void CommandRecordingContext::SetDescriptorHeaps(ID3D12DescriptorHeap* resourceHeap, ID3D12DescriptorHeap* samplerHeap) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		ID3D12DescriptorHeap* heaps[] = { resourceHeap, samplerHeap };
		m_CommandList->SetDescriptorHeaps(2, heaps);
	}

	void CommandRecordingContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView, float* clearColor) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->ClearRenderTargetView(renderTargetView, clearColor, 0, nullptr);
	}

	void CommandRecordingContext::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsv, float depth, uint8_t stencil) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
	}

	void CommandRecordingContext::SetRenderTargets(std::span<const D3D12_CPU_DESCRIPTOR_HANDLE> rtvRange, std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> dsv) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->OMSetRenderTargets(static_cast<UINT>(rtvRange.size()), rtvRange.data(), true, dsv.has_value() ? &dsv.value() : nullptr);
	}

	void CommandRecordingContext::SetPipelineState(ID3D12PipelineState* pipelineState) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->SetPipelineState(pipelineState);
	}

	void CommandRecordingContext::SetGraphicsRootSignature(ID3D12RootSignature* rootSignature) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->SetGraphicsRootSignature(rootSignature);
	}

	void CommandRecordingContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->IASetPrimitiveTopology(topology);
	}

	void CommandRecordingContext::SetViewports(std::span<const D3D12_VIEWPORT> viewports) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
	}

	void CommandRecordingContext::SetScissorRects(std::span<const D3D12_RECT> rects) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
	}

	void CommandRecordingContext::SetVertexBuffers(uint32_t startSlot, std::span<const D3D12_VERTEX_BUFFER_VIEW> vertexBuffers) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->IASetVertexBuffers(startSlot, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data());
	}

	void CommandRecordingContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBuffer) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->IASetIndexBuffer(&indexBuffer);
	}

	void CommandRecordingContext::DrawInstanced(uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertex, uint32_t startInstance) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->DrawInstanced(vertexCountPerInstance, instanceCount, startVertex, startInstance);
	}

	void CommandRecordingContext::DrawIndexedInstanced(uint32_t indexCountPerInstance, uint32_t instanceCount, uint32_t startIndex, uint32_t startVertex, uint32_t startInstance) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndex, startVertex, startInstance);
	}

}
