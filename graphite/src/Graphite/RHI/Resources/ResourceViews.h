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
	};


	struct IndexBufferView
	{
		GPUVirtualAddress BufferAddress;
		uint64_t BufferSize;
		GraphiteFormat IndexFormat;

		GRAPHITE_API static IndexBufferView Create(const UploadBuffer& resource);
		GRAPHITE_API static IndexBufferView Create(const StructuredBuffer& resource);
		GRAPHITE_API static IndexBufferView Create(const GPUResource& resource);
	};

	/*
	struct ConstantBufferView
	{
		GPUResource* Resource;

		GPUVirtualAddress BufferLocation;
		uint32_t SizeInBytes;
	};

	struct ShaderResourceView
	{
		GPUResource* Resource;

		GraphiteFormat Format;
		// Resource-type-dependent properties
	};

	struct UnorderedAccessView
	{
		GPUResource* Resource;


	};

	struct RenderTargetView
	{
		
	};

	struct DepthStencilView
	{
		
	};
	*/

}
