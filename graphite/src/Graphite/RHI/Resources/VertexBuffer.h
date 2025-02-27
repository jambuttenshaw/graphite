#pragma once

#include "GPUResource.h"
#include "Core/Core.h"

#include "RHI/RHITypes.h"
#include "InputLayout.h"
#include "ResourceViews.h"


namespace Graphite
{
	class GPUResource;

	class UploadBuffer;
	class StructuredBuffer;


	class VertexBuffer : public GPUResource
	{
		// Allocates a vertex buffer with the specified attributes
		friend class ResourceFactory;
		VertexBuffer(uint32_t vertexCount, const InputLayout& layout);
	public:
		virtual ~VertexBuffer() = default;

		DELETE_COPY(VertexBuffer)
		DEFAULT_MOVE(VertexBuffer)

		virtual GPUResourceType GetResourceType() const override { return GPUResourceType::VertexBuffer; }

	protected:
		void CreateViews();

	protected:
		uint32_t m_VertexCount;
		const InputLayout* m_InputLayout = nullptr;

		// When vertex data is not interleaved, multiple buffer views are required
		// e.g., an array of positions followed by an array of UV's and so on
		// Thus a different buffer view is required for each attribute
		std::vector<VertexBufferView> m_BufferViews;
	};
}
