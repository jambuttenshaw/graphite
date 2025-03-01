#pragma once

#include "Buffer.h"


namespace Graphite
{
	class GraphicsContext;

	class VertexBuffer;
	class InputLayout;


	class ResourceFactory
	{
	protected:
		GRAPHITE_API ResourceFactory() = default;
	public:
		// Allow client applications to access resource factory
		GRAPHITE_API static ResourceFactory& Get();

	public:
		GRAPHITE_API virtual ~ResourceFactory() = default;

		GRAPHITE_API_DELETE_COPY(ResourceFactory);
		GRAPHITE_API_DELETE_MOVE(ResourceFactory);

		// Resource Factory

		// Buffers

		// Instanced buffers are used to allow safe writing of memory from the CPU each frame through a common object
		// The client writing to the buffer doesn't have to worry about the underlying structure of the resource
		GRAPHITE_API virtual std::unique_ptr<UploadBuffer> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize, uint32_t elementAlignment) const = 0;
		GRAPHITE_API virtual std::unique_ptr<ByteAddressBuffer> CreateByteAddressBuffer(uint64_t width, bool readOnly) const = 0;
		GRAPHITE_API virtual std::unique_ptr<StructuredBuffer> CreateStructuredBuffer(uint32_t elementCount, uint32_t elementSize, bool readOnly) const = 0;
		GRAPHITE_API virtual std::unique_ptr<ReadbackBuffer> CreateReadbackBuffer(uint64_t width) const = 0;

		GRAPHITE_API virtual std::unique_ptr<VertexBuffer> CreateVertexBuffer(uint32_t vertexCount, const InputLayout& inputLayout) const = 0;

		// Templated variations to automatically determine sizes
		template <typename T>
		std::unique_ptr<UploadBuffer> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementAlignment) const
		{
			return CreateUploadBuffer(elementCount, instanceCount, sizeof(T), elementAlignment);
		}

		template <typename T>
		std::unique_ptr<StructuredBuffer> CreateStructuredBuffer(uint32_t elementCount, bool readOnly) const
		{
			return CreateStructuredBuffer(elementCount, sizeof(T), readOnly);
		}

	protected:
		// Helper functions
		GRAPHITE_API static uint64_t AlignSize(uint64_t size, uint64_t alignment);
	};
}
