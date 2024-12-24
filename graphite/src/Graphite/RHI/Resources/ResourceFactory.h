#pragma once

#include "Buffer.h"
#include "D3D12MemAlloc.h"
#include "Graphite/RHI/RHIExceptions.h"


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
		template <typename T>
		std::unique_ptr<UploadBuffer<T>> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementAlignment)
		{
			// Calculate properties of the required memory

			// Align the element stride to be a multiple of alignment
			uint64_t elementStride = sizeof(T);
			if (elementAlignment > 0)
			{
				AlignSize(elementStride, elementAlignment);
			}
			const uint64_t width = elementStride * elementCount * instanceCount;

			// Allocate memory for the buffer
			// Flags don't apply to default heap memory
			const auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(width, D3D12_RESOURCE_FLAG_NONE, 0);
			D3D12MA::ALLOCATION_DESC allocDesc = {};
			allocDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

			// Allocate memory
			// TODO: Don't throw on failure
			D3D12MA::Allocation* allocation;
			DX_THROW_IF_FAIL(m_Allocator->CreateResource(
				&allocDesc,
				&resourceDesc,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				&allocation,
				IID_NULL, nullptr);)

			auto buffer = std::unique_ptr<UploadBuffer<T>>(new UploadBuffer<T>(allocation, elementCount, instanceCount, static_cast<uint32_t>(elementStride)));
			return std::move(buffer);
		}

		template <typename T>
		using ConstantBuffer = UploadBuffer<T>;
		// Constant Buffers are upload buffers with alignment requirements
		template <typename T>
		std::unique_ptr<ConstantBuffer<T>> CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount)
		{
			return std::move(CreateUploadBuffer<T>(elementCount, instanceCount, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT));
		}

		// ByteAddressBuffer/DefaultBuffer
		void CreateByteAddress();
		void CreateStructuredBuffer();
		void CreateReadbackBuffer();

		// Textures
		void CreateTexture();

	private:
		static uint64_t AlignSize(uint64_t size, uint64_t alignment);

	private:
		D3D12MA::Allocator* m_Allocator = nullptr;
	};
}
