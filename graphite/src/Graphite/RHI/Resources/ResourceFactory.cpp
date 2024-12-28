#include "graphite_pch.h"
#include "ResourceFactory.h"

#include "Graphite/RHI/RHIExceptions.h"
#include "Graphite/Core/Assert.h"

#include "D3D12MemAlloc.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{
	ResourceFactory& ResourceFactory::Get()
	{
		static ResourceFactory instance;
		return instance;
	}


	void ResourceFactory::CreateResourceAllocator(IDXGIAdapter* adapter, ID3D12Device* device)
	{
		D3D12MA::ALLOCATOR_DESC desc = {};
		desc.pDevice = device;
		desc.pAdapter = adapter;
		desc.Flags = D3D12MA::ALLOCATOR_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED | D3D12MA::ALLOCATOR_FLAG_DEFAULT_POOLS_NOT_ZEROED;

		DX_THROW_IF_FAIL(D3D12MA::CreateAllocator(&desc, &m_Allocator));
	}

	void ResourceFactory::DestroyResourceAllocator()
	{
		GRAPHITE_ASSERT(m_Allocator, "Cannot destroy allocator - CreateResourceAllocator was never called!");
		m_Allocator->Release();
	}


	std::unique_ptr<UploadBuffer> ResourceFactory::CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize, uint32_t elementAlignment) const
	{
		// Calculate properties of the required memory

		// Align the element stride to be a multiple of alignment
		uint64_t elementStride = elementSize;
		if (elementAlignment > 0)
		{
			AlignSize(elementStride, elementAlignment);
		}
		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_UPLOAD,
			elementStride * elementCount * instanceCount,
			D3D12_RESOURCE_FLAG_NONE);

		auto buffer = std::unique_ptr<UploadBuffer>(new UploadBuffer(allocation, elementCount, instanceCount, static_cast<uint32_t>(elementStride)));
		return std::move(buffer);
	}


	std::unique_ptr<ConstantBuffer> ResourceFactory::CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize) const
	{
		// Calculate properties of the required memory

		// Align the element stride to be a multiple of alignment
		const uint64_t elementStride = AlignSize(elementSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_UPLOAD,
			elementStride * elementCount * instanceCount,
			D3D12_RESOURCE_FLAG_NONE);

		auto buffer = std::unique_ptr<ConstantBuffer>(new ConstantBuffer(allocation, elementCount, instanceCount, static_cast<uint32_t>(elementStride)));
		return std::move(buffer);
	}


	std::unique_ptr<ByteAddressBuffer> ResourceFactory::CreateByteAddressBuffer(uint64_t width, bool readOnly) const
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		if (!readOnly)
		{
			flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}

		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_DEFAULT,
			width,
			flags);

		auto buffer = std::unique_ptr<ByteAddressBuffer>(new ByteAddressBuffer(allocation, readOnly));
		return std::move(buffer);
	}


	std::unique_ptr<StructuredBuffer> ResourceFactory::CreateStructuredBuffer(uint32_t elementCount, uint32_t elementSize, bool readOnly) const
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		if (!readOnly)
		{
			flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}

		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_DEFAULT,
			static_cast<uint64_t>(elementCount * elementSize), 
			flags);

		auto buffer = std::unique_ptr<StructuredBuffer>(new StructuredBuffer(allocation, elementCount, readOnly));
		return std::move(buffer);
	}


	std::unique_ptr<ReadbackBuffer> ResourceFactory::CreateReadbackBuffer(uint64_t width) const
	{
		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_READBACK,
			width,
			D3D12_RESOURCE_FLAG_NONE);

		auto buffer = std::unique_ptr<ReadbackBuffer>(new ReadbackBuffer(allocation));
		return std::move(buffer);
	}


	std::unique_ptr<VertexBuffer> ResourceFactory::CreateVertexBuffer(uint32_t vertexCount, uint32_t vertexStride, bool dynamic) const
	{
		auto heapType = (dynamic) ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;
		uint64_t width = static_cast<uint64_t>(vertexCount * vertexStride);

		auto allocation = AllocateBuffer(
			heapType,
			width,
			D3D12_RESOURCE_FLAG_NONE
		);

		auto buffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(allocation, vertexCount, vertexStride, dynamic));
		return std::move(buffer);
	}

	std::unique_ptr<IndexBuffer> ResourceFactory::CreateIndexBuffer(uint32_t indexCount, uint32_t indexSizeInBytes, bool dynamic) const
	{
		auto heapType = (dynamic) ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;
		uint64_t width = static_cast<uint64_t>(indexCount * indexSizeInBytes);

		auto allocation = AllocateBuffer(
			heapType,
			width,
			D3D12_RESOURCE_FLAG_NONE
		);

		auto buffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(allocation, indexCount, indexSizeInBytes, dynamic));
		return std::move(buffer);
	}


	// --- Helper Functions ---

	uint64_t ResourceFactory::AlignSize(uint64_t size, uint64_t alignment)
	{
		return (size + (alignment - 1)) & ~(alignment - 1);
	}

	D3D12MA::Allocation* ResourceFactory::AllocateBuffer(D3D12_HEAP_TYPE heap, uint64_t width, D3D12_RESOURCE_FLAGS flags) const
	{
		const auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(width, flags, 0);
		D3D12MA::ALLOCATION_DESC allocDesc = {};
		allocDesc.HeapType = heap;

		// Allocate memory
		// TODO: Don't throw on failure
		D3D12MA::Allocation* allocation;
		DX_THROW_IF_FAIL(m_Allocator->CreateResource(
			&allocDesc,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			&allocation,
			IID_NULL, nullptr));
		return allocation;
	}



}
