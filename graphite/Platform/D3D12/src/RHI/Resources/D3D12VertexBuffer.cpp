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
		std::ranges::transform(m_BufferViews, m_NativeViews.begin(), 
		[](const VertexBufferView& view)
		{
			return GraphiteVBVToD3D12VBV(view);
		});
	}

	D3D12VertexBuffer::~D3D12VertexBuffer()
	{
		if (m_MappedData)
		{
			m_Resource->Unmap(0, nullptr);
		}
	}

	void D3D12VertexBuffer::CopyAttribute(VertexAttribute attribute, const void* data, uint32_t elementStride, size_t elementCount)
	{
		const InputLayout::InputElement& inputElement = m_InputLayout->GetInputElement(attribute);
		GRAPHITE_ASSERT(inputElement.SizeInBytes == elementStride, "Attribute size mismatch.");

		if (m_InputLayout->IsInterleaved())
		{
			GRAPHITE_LOG_WARN("Copying single attributes for is sub-optimal for interleaved vertex buffers.");
			for (size_t i = 0; i < elementCount; i++)
			{
				memcpy(
					m_MappedData + (i * m_InputLayout->GetLayoutSizeInBytes()) + inputElement.OffsetInBytes,
					static_cast<const uint8_t*>(data) + (i * elementStride),
					elementStride
				);
			}
		}
		else
		{
			memcpy(
				m_MappedData + static_cast<uint64_t>(m_VertexCount * inputElement.OffsetInBytes),
				data,
				elementCount * elementStride
			);
		}
	}


}
