#pragma once

#include "Buffer.h"


namespace Graphite
{
	class InputLayout;


	// Vertex and index buffer views are simpler because they are CPU-only resources and do not have underlying descriptors

	struct VertexBufferView
	{
		GPUVirtualAddress BufferAddress;
		uint64_t BufferSize;
		uint64_t VertexStride;

		static VertexBufferView Create(const UploadBuffer& resource);
		static VertexBufferView Create(const StructuredBuffer& resource);
		static VertexBufferView Create(const GPUResource& resource, const InputLayout& vertexLayout);
	};


	struct IndexBufferView
	{
		GPUVirtualAddress BufferAddress;
		uint64_t BufferSize;
		GraphiteFormat IndexFormat;

		static IndexBufferView Create(const UploadBuffer& resource);
		static IndexBufferView Create(const StructuredBuffer& resource);
		static IndexBufferView Create(const GPUResource& resource);
	};


	// Other types of views own state - they must be managed more carefully
	class RenderTargetView
	{
		
	};

}
