#pragma once

#include "Buffer.h"
#include "Geometry.h"
#include "Graphite/RHI/RHIExceptions.h"

namespace D3D12MA
{
	class Allocator;
	class Allocation;
}

namespace Graphite
{
	class ResourceFactory
	{
		// Private constructor - must be accessed through Get()
		ResourceFactory() = default;
	public:
		static ResourceFactory& Get();

		~ResourceFactory() = default;

		DELETE_COPY(ResourceFactory);
		DELETE_MOVE(ResourceFactory);

		// Initialization
		void CreateResourceAllocator(IDXGIAdapter* adapter, ID3D12Device* device);
		void DestroyResourceAllocator();

		// Resource Factory

		// Buffers

		// Instanced buffers are used to allow safe writing of memory from the CPU each frame through a common object
		// The client writing to the buffer doesn't have to worry about the underlying structure of the resource
		std::unique_ptr<UploadBuffer> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize, uint32_t elementAlignment) const;
		template <typename T>
		std::unique_ptr<UploadBuffer> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementAlignment) const
		{
			return CreateUploadBuffer(elementCount, instanceCount, sizeof(T), elementAlignment);
		}


		std::unique_ptr<ConstantBuffer> CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize) const;
		template <typename T>
		std::unique_ptr<UploadBuffer> CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount) const
		{
			return CreateConstantBuffer(elementCount, instanceCount, sizeof(T));
		}


		std::unique_ptr<ByteAddressBuffer> CreateByteAddressBuffer(uint64_t width, bool readOnly) const;


		std::unique_ptr<StructuredBuffer> CreateStructuredBuffer(uint32_t elementCount, uint32_t elementSize, bool readOnly) const;
		template <typename T>
		std::unique_ptr<UploadBuffer> CreateStructuredBuffer(uint32_t elementCount, bool readOnly) const
		{
			return CreateStructuredBuffer(elementCount, sizeof(T), readOnly);
		}
		

		std::unique_ptr<ReadbackBuffer> CreateReadbackBuffer(uint64_t width) const;


		// Vertex / Index
		std::unique_ptr<VertexBuffer> CreateVertexBuffer(uint32_t vertexCount, uint32_t vertexStride, bool dynamic) const;
		std::unique_ptr<IndexBuffer> CreateIndexBuffer(uint32_t indexCount, uint32_t indexSizeInBytes, bool dynamic) const;


		// Textures
		void CreateTexture();

	private:
		// Helper functions
		static uint64_t AlignSize(uint64_t size, uint64_t alignment);

		D3D12MA::Allocation* AllocateBuffer(D3D12_HEAP_TYPE heap, uint64_t width, D3D12_RESOURCE_FLAGS flags) const;
	private:
		D3D12MA::Allocator* m_Allocator = nullptr;
	};
}
