#include "graphite_pch.h"
#include "Geometry.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/Core/Log.h"
#include "Graphite/RHI/RHIExceptions.h"


namespace Graphite
{
	const InputLayout Vertex_Position::VertexInputLayout({
		{"POSITION", 0, 3},
	});

	const InputLayout Vertex_PositionNormalUV::VertexInputLayout({
		{"POSITION", 0, 3},
		{"NORMAL", 0, 3},
		{"TEXCOORD", 0, 2}
	});


	VertexBuffer::VertexBuffer(D3D12MA::Allocation* allocation, uint32_t vertexCount, uint32_t vertexStride, bool dynamic)
		: GPUResource(
			allocation,
			static_cast<ResourceAccessFlags>(ResourceAccess_GPURead | (dynamic ? ResourceAccess_CPUWrite : ResourceAccess_None))
			)
		, m_VertexCount(vertexCount)
		, m_VertexStride(vertexStride)
	{
		m_VertexBufferView = D3D12_VERTEX_BUFFER_VIEW{
			.BufferLocation = GetAddress(),
			.SizeInBytes = vertexCount * vertexStride,
			.StrideInBytes = vertexStride
		};
	}

	void VertexBuffer::CopyVertexData(uint32_t vertexCount, const void* vertexData) const
	{
		GRAPHITE_ASSERT(CheckAccessFlags(ResourceAccess_CPUWrite), "Cannot copy to static vertex buffer - use a staging resource instead.");
		GRAPHITE_ASSERT(vertexCount <= m_VertexCount, "Invalid vertex count!");

		// Map buffer
		uint8_t* mappedData;
		DX_THROW_IF_FAIL(m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));

		// Copy vertex data into the buffer
		uint64_t size = vertexCount * m_VertexStride;
		memcpy(mappedData, vertexData, size);

		m_Resource->Unmap(0, nullptr);
	}


	IndexBuffer::IndexBuffer(D3D12MA::Allocation* allocation, uint32_t indexCount, uint32_t indexSizeInBytes, bool dynamic)
		: GPUResource(
			allocation,
			static_cast<ResourceAccessFlags>(ResourceAccess_GPURead | (dynamic ? ResourceAccess_CPUWrite : ResourceAccess_None))
		)
		, m_IndexCount(indexCount)
		, m_IndexSizeInBytes(indexSizeInBytes)
	{
		m_IndexBufferView = D3D12_INDEX_BUFFER_VIEW{
			.BufferLocation = GetAddress(),
			.SizeInBytes = indexCount * indexSizeInBytes,
			.Format = IndexSizeInBytesToFormat(indexSizeInBytes)
		};
	}

	void IndexBuffer::CopyIndexData(uint32_t indexCount, const void* indexData) const
	{
		GRAPHITE_ASSERT(CheckAccessFlags(ResourceAccess_CPUWrite), "Cannot copy to static index buffer - use a staging resource instead.");
		GRAPHITE_ASSERT(indexCount <= m_IndexCount, "Invalid vertex count!");

		// Map buffer
		uint8_t* mappedData;
		DX_THROW_IF_FAIL(m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));

		// Copy vertex data into the buffer
		uint64_t size = indexCount * m_IndexSizeInBytes;
		memcpy(mappedData, indexData, size);

		m_Resource->Unmap(0, nullptr);
	}


	DXGI_FORMAT IndexBuffer::IndexSizeInBytesToFormat(uint32_t sizeInBytes)
	{
		switch (sizeInBytes)
		{
		case 1:
			return DXGI_FORMAT_R8_UINT;
		case 2:
			return DXGI_FORMAT_R16_UINT;
		case 4:
			return DXGI_FORMAT_R32_UINT;
		default:
			GRAPHITE_LOG_ERROR("Invalid index size: {} bytes.", sizeInBytes);
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}
