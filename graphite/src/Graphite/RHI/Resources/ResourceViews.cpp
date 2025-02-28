#include "graphite_pch.h"
#include "ResourceViews.h"

#include "InputLayout.h"
#include "Graphite/Core/Assert.h"

namespace Graphite
{

	static GraphiteFormat IndexStrideToBufferFormat(uint32_t stride)
	{
		switch (stride)
		{
		case 1: return GraphiteFormat_R8_UINT;
		case 2: return GraphiteFormat_R16_UINT;
		case 4: return GraphiteFormat_R32_UINT;
		default:
			GRAPHITE_LOG_ERROR("Invalid index stride: {0}", stride);
			break;
		}

		return GraphiteFormat_Unknown;
	}

	/*
	VertexBufferView VertexBufferView::Create(const GPUResource& resource, const InputLayout& vertexLayout)
	{
		GRAPHITE_ASSERT(false, "Not implemented");
		return VertexBufferView{
			.BufferAddress = resource.GetResourceAddress(),
			.BufferSize = 0, //TODO: Need a way of passing through buffer size
			.VertexStride = vertexLayout.GetLayoutSizeInBytes()
		};
	}
	*/

	IndexBufferView IndexBufferView::Create(const UploadBuffer& resource)
	{
		return IndexBufferView{
			.BufferAddress = resource.GetResourceAddress(),
			.BufferSize = resource.GetElementCount() * resource.GetElementStride(),
			.IndexFormat = IndexStrideToBufferFormat(resource.GetElementStride())
		};
	}

	IndexBufferView IndexBufferView::Create(const StructuredBuffer& resource)
	{
		return IndexBufferView{
			.BufferAddress = resource.GetResourceAddress(),
			.BufferSize = resource.GetElementCount() * resource.GetElementSize(),
			.IndexFormat = IndexStrideToBufferFormat(resource.GetElementSize())
		};
	}

	IndexBufferView IndexBufferView::Create(const GPUResource& resource)
	{
		GRAPHITE_ASSERT(false, "Not implemented");
		return IndexBufferView{
			.BufferAddress = resource.GetResourceAddress(),
			.BufferSize = 0, //TODO: Need a way of passing through buffer size
			.IndexFormat = GraphiteFormat_R16_UINT
		};
	}


}
