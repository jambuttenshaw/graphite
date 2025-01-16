#include "graphite_pch.h"
#include "D3D12CommandRecordingContext.h"

#include "Graphite/Core/Assert.h"

#include "D3D12Exceptions.h"
#include "D3D12DescriptorHeap.h"
#include "D3D12Types.h"

#include "Pipelines/D3D12GraphicsPipeline.h"
#include "Resources/D3D12ResourceViews.h"


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

	void D3D12CommandRecordingContext::Reset(void* allocator)
	{
		DX_THROW_IF_FAIL(m_CommandList->Reset(static_cast<ID3D12CommandAllocator*>(allocator), nullptr));
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
			dynamic_cast<D3D12DescriptorHeap*>(resourceHeap)->GetNativeHeap(),
			dynamic_cast<D3D12DescriptorHeap*>(samplerHeap)->GetNativeHeap()
		};
		m_CommandList->SetDescriptorHeaps(2, heaps);
	}

	void D3D12CommandRecordingContext::ClearRenderTargetView(CPUDescriptorHandle renderTargetView, const glm::vec4& clearColor) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		auto descriptor = GraphiteCPUDescriptorToD3D12Descriptor(renderTargetView);
		m_CommandList->ClearRenderTargetView(descriptor, &clearColor.x, 0, nullptr);
	}

	void D3D12CommandRecordingContext::ClearDepthStencilView(CPUDescriptorHandle dsv, float depth, uint8_t stencil) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		auto descriptor = GraphiteCPUDescriptorToD3D12Descriptor(dsv);
		m_CommandList->ClearDepthStencilView(descriptor, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
	}

	void D3D12CommandRecordingContext::SetRenderTargets(uint32_t rtvCount, CPUDescriptorHandle rtvRange, std::optional<CPUDescriptorHandle> dsv) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");

		auto rtvDescriptor = GraphiteCPUDescriptorToD3D12Descriptor(rtvRange);

		D3D12_CPU_DESCRIPTOR_HANDLE dsvDescriptor;
		if (dsv.has_value())
		{
			dsvDescriptor = GraphiteCPUDescriptorToD3D12Descriptor(dsv.value());
		}

		m_CommandList->OMSetRenderTargets(static_cast<UINT>(rtvCount), &rtvDescriptor, true, dsv.has_value() ? &dsvDescriptor : nullptr);
	}

	void D3D12CommandRecordingContext::SetGraphicsPipelineState(const GraphicsPipeline& pipelineState) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		const D3D12GraphicsPipeline& nativePipeline = dynamic_cast<const D3D12GraphicsPipeline&>(pipelineState);
		m_CommandList->SetPipelineState(nativePipeline.GetPipelineState());
		m_CommandList->SetGraphicsRootSignature(nativePipeline.GetRootSignature());
	}

	void D3D12CommandRecordingContext::SetGraphicsPipelineResources(const ResourceViewList& resourceViewList) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		const auto& resourceSet = resourceViewList.GetPipelineResourceSet();

		uint32_t argIndex = resourceSet.GetBaseRootArgumentIndex();
		uint32_t i = 0;

		for (const auto& arg : resourceSet.GetRootArguments())
		{
			if (arg.BindingMethod == PipelineResourceBindingMethod::Default)
			{
				m_CommandList->SetGraphicsRootDescriptorTable(
					argIndex + i,
					GraphiteGPUDescriptorToD3D12Descriptor(resourceViewList.GetDescriptorTableHandle())
				);
			}
			else
			{
				uint32_t idx = argIndex + i;
				D3D12_GPU_VIRTUAL_ADDRESS va = GraphiteGPUAddressToD3D12GPUAddress(resourceViewList.GetInlineResourceHandle(arg.InlineResourceOffset));
				switch (arg.Type)
				{
				case PipelineResourceType::ConstantBufferView:
					m_CommandList->SetGraphicsRootConstantBufferView(idx, va);
					break;
				case PipelineResourceType::ShaderResourceView:
					m_CommandList->SetGraphicsRootShaderResourceView(idx, va);
					break;
				case PipelineResourceType::UnorderedAccessView:
					m_CommandList->SetGraphicsRootUnorderedAccessView(idx, va);
					break;
				case PipelineResourceType::Invalid:
				default:
					GRAPHITE_LOG_FATAL("Unknown resource type encountered!");
				}
			}
			i++;
		}
	}

	void D3D12CommandRecordingContext::SetPrimitiveTopology(GraphiteTopology topology) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->IASetPrimitiveTopology(GraphiteTopologyToD3D12Topology(topology));
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

	void D3D12CommandRecordingContext::SetVertexBuffers(uint32_t startSlot, std::span<const VertexBufferView> vertexBuffers) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");

		std::vector<D3D12_VERTEX_BUFFER_VIEW> vbs(vertexBuffers.size());
		for (size_t i = 0; i < vbs.size(); i++)
		{
			vbs.at(i) = GraphiteVBVToD3D12VBV(vertexBuffers[i]);
		}

		m_CommandList->IASetVertexBuffers(startSlot, static_cast<UINT>(vbs.size()), vbs.data());
	}

	void D3D12CommandRecordingContext::SetIndexBuffer(const IndexBufferView& indexBuffer) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		D3D12_INDEX_BUFFER_VIEW ibv = GraphiteIBVToD3D12IBV(indexBuffer);
		m_CommandList->IASetIndexBuffer(&ibv);
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
