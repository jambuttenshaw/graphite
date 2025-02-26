#pragma once

#include "Core/Core.h"

#include "RHI/RHITypes.h"
#include "InputLayout.h"


namespace Graphite
{
	class GPUResource;

	class UploadBuffer;
	class StructuredBuffer;


	enum class VertexAttribute : int8_t
	{
		Invalid = -1,
		// Begin attributes
		Position,
		UV,
		Normal,
		Tangent,
		Color,
		Custom,
		// End attributes
		Count
	};


	struct VertexBufferView
	{
		GPUVirtualAddress BufferAddress;
		uint64_t BufferSize;
		uint64_t VertexStride;

		GRAPHITE_API static VertexBufferView Create(const UploadBuffer& resource);
		GRAPHITE_API static VertexBufferView Create(const StructuredBuffer& resource);
		GRAPHITE_API static VertexBufferView Create(const GPUResource& resource, const InputLayout& vertexLayout);
	};


	class VertexBuffer
	{
	public:
		// Allocates a vertex buffer with the specified attributes
		VertexBuffer(uint32_t vertexCount);

		void AddVertexAttribute(VertexAttribute attributeSlot, GraphiteFormat vertexFormat);

		// Underlying buffer will be allocated once all vertex attributes are described
		void AllocateBuffer();

		// For contiguous data, set stride to 0 or attributeStrideInBytes (the size is the minimum stride anyway)
		void CopyVertexData(VertexAttribute attributeSlot, uint32_t attributeSizeInBytes, uint32_t attributeStrideInBytes, uint8_t* data);

	private:
		// Whenever the input layout or underlying buffer is changed, the views will be invalidated and recreated
		void CreateViews();

	private:
		uint32_t m_VertexCount;

		// Internal buffer on the GPU to hold vertex data
		// TODO: Support default/structured buffers
		// TODO: This is useful both for faster read access when rendering, but also allows
		// TODO: manipulation and generation of vertices on the GPU
		// TODO: Manipulation of non-interleaved vertices will be quite annoying.
		// TODO: Interleaving could be disabled by default, but also possible to use
		std::unique_ptr<UploadBuffer> m_Buffer;

		InputLayout m_InputLayout;
		// Vertex data is NOT interleaved, but IS contiguous
		// e.g., an array of positions followed by an array of UV's and so on
		// Thus a different buffer view is required for each attribute
		std::vector<VertexBufferView> m_BufferViews;
	};
}
