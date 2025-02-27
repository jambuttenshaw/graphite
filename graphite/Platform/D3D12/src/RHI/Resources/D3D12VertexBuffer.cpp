#include "graphite_d3d12_pch.h"
#include "D3D12VertexBuffer.h"

#include "RHI/D3D12Exceptions.h"
#include "RHI/D3D12Types.h"


namespace Graphite::D3D12
{

	D3D12VertexBuffer::D3D12VertexBuffer(D3D12MA::Allocation* allocation, uint32_t vertexCount, const InputLayout& inputLayout)
		: VertexBuffer(vertexCount, inputLayout)
		, D3D12Resource(allocation)
	{
		DX_THROW_IF_FAIL(m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));

		CreateViews();
		// Create native vertex buffer views
		m_NativeViews.resize(m_BufferViews.size());
		std::ranges::views::transform(m_BufferViews, )

		for (auto& view : m_NativeViews)
		{
			view = GraphiteVBVToD3D12VBV()
		}
	}

	D3D12VertexBuffer::~D3D12VertexBuffer()
	{
		if (m_MappedData)
		{
			m_Resource->Unmap(0, nullptr);
		}
	}



}
