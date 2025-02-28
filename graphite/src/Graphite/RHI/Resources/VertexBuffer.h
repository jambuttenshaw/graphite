#pragma once

#include "GPUResource.h"
#include "Graphite/Core/Core.h"

#include "InputLayout.h"
#include "ResourceViews.h"
#include "Graphite/Core/Assert.h"


namespace Graphite
{
	class GPUResource;

	class UploadBuffer;
	class StructuredBuffer;


	class VertexBuffer : public GPUResource
	{
	protected:
		// Allocates a vertex buffer with the specified attributes
		friend class ResourceFactory;
		GRAPHITE_API VertexBuffer(uint32_t vertexCount, const InputLayout& layout);
	public:
		GRAPHITE_API virtual ~VertexBuffer() = default;

		GRAPHITE_API_DELETE_COPY(VertexBuffer)
		GRAPHITE_API_DEFAULT_MOVE(VertexBuffer)

		GRAPHITE_API virtual GPUResourceType GetResourceType() const override { return GPUResourceType::VertexBuffer; }

		// Copy data interface
		template<typename T>
		void CopyAttribute(VertexAttribute attribute, std::span<const T> data)
		{
			GRAPHITE_ASSERT(m_InputLayout->HasAttribute(attribute), "Vertex buffer does not contain attribute.");
			GRAPHITE_ASSERT(data.size() <= m_VertexCount, "Vertex buffer overflow.");

			const InputLayout::InputElement& element = m_InputLayout->GetInputElement(attribute);
			GRAPHITE_ASSERT(sizeof(T) == element.SizeInBytes, "Element format mismatch.");

			CopyAttribute(attribute, data.data(), element.SizeInBytes, data.size());
		}

		GRAPHITE_API virtual void CopyAttribute(VertexAttribute attribute, const void* data, uint32_t elementStride, size_t elementCount) = 0;

	protected:
		GRAPHITE_API void CreateViews();

	protected:
		uint32_t m_VertexCount;
		const InputLayout* m_InputLayout = nullptr;

		// When vertex data is not interleaved, multiple buffer views are required
		// e.g., an array of positions followed by an array of UV's and so on
		// Thus a different buffer view is required for each attribute
		std::vector<VertexBufferView> m_BufferViews;
	};
}
