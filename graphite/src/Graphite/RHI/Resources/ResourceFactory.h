#pragma once

#include "Buffer.h"


namespace Graphite
{
	class GraphicsContext;

	class ResourceFactory
	{
		// Singleton
		static std::unique_ptr<ResourceFactory> s_ResourceFactory;

	protected:
		ResourceFactory() = default;
	public:
		static void CreateResourceFactory(const GraphicsContext& graphicsContext);
		static void DestroyResourceFactory();

		// Allow client applications to access resource factory
		GRAPHITE_API static ResourceFactory& Get();

	public:
		virtual ~ResourceFactory() = default;

		DELETE_COPY(ResourceFactory);
		DELETE_MOVE(ResourceFactory);

		// Resource Factory

		// Buffers

		// Instanced buffers are used to allow safe writing of memory from the CPU each frame through a common object
		// The client writing to the buffer doesn't have to worry about the underlying structure of the resource
		GRAPHITE_API virtual std::unique_ptr<UploadBuffer> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize, uint32_t elementAlignment) const = 0;
		GRAPHITE_API virtual std::unique_ptr<ConstantBuffer> CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize) const = 0;
		GRAPHITE_API virtual std::unique_ptr<ByteAddressBuffer> CreateByteAddressBuffer(uint64_t width, bool readOnly) const = 0;
		GRAPHITE_API virtual std::unique_ptr<StructuredBuffer> CreateStructuredBuffer(uint32_t elementCount, uint32_t elementSize, bool readOnly) const = 0;
		GRAPHITE_API virtual std::unique_ptr<ReadbackBuffer> CreateReadbackBuffer(uint64_t width) const = 0;

		// Templated variations to automatically determine sizes
		template <typename T>
		std::unique_ptr<UploadBuffer> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementAlignment) const
		{
			return CreateUploadBuffer(elementCount, instanceCount, sizeof(T), elementAlignment);
		}

		template <typename T>
		std::unique_ptr<UploadBuffer> CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount) const
		{
			return CreateConstantBuffer(elementCount, instanceCount, sizeof(T));
		}

		template <typename T>
		std::unique_ptr<UploadBuffer> CreateStructuredBuffer(uint32_t elementCount, bool readOnly) const
		{
			return CreateStructuredBuffer(elementCount, sizeof(T), readOnly);
		}

	protected:
		// Helper functions
		static uint64_t AlignSize(uint64_t size, uint64_t alignment);
	};
}
