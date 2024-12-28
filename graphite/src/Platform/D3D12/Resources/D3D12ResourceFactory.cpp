#include "graphite_pch.h"
#include "D3D12ResourceFactory.h"

#include "D3D12MemAlloc.h"

#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/Resources/ResourceFactory.h"

#include "Platform/D3D12/D3D12GraphicsContext.h"
#include "Platform/D3D12/D3D12Exceptions.h"
#include "D3D12Buffer.h"


namespace Graphite::D3D12
{

	D3D12ResourceFactory::D3D12ResourceFactory(const GraphicsContext& graphicsContext)
		: ResourceFactory()
	{
		const D3D12GraphicsContext& nativeGraphicsContext = dynamic_cast<const D3D12GraphicsContext&>(graphicsContext);

		D3D12MA::ALLOCATOR_DESC desc = {};
		desc.pDevice = nativeGraphicsContext.GetDevice();
		desc.pAdapter = nativeGraphicsContext.GetAdapter();
		desc.Flags = D3D12MA::ALLOCATOR_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED | D3D12MA::ALLOCATOR_FLAG_DEFAULT_POOLS_NOT_ZEROED;

		DX_THROW_IF_FAIL(D3D12MA::CreateAllocator(&desc, &m_Allocator));
	}

	D3D12ResourceFactory::~D3D12ResourceFactory()
	{
		GRAPHITE_ASSERT(m_Allocator, "Cannot destroy allocator - CreateResourceAllocator was never called!");
		m_Allocator->Release();
	}


	std::unique_ptr<UploadBuffer> D3D12ResourceFactory::CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize, uint32_t elementAlignment) const
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

		auto buffer = std::unique_ptr<UploadBuffer>(new D3D12UploadBuffer(allocation, elementCount, instanceCount, static_cast<uint32_t>(elementStride)));
		return std::move(buffer);
	}


	std::unique_ptr<ConstantBuffer> D3D12ResourceFactory::CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize) const
	{
		// Calculate properties of the required memory

		// Align the element stride to be a multiple of alignment
		const uint64_t elementStride = AlignSize(elementSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_UPLOAD,
			elementStride * elementCount * instanceCount,
			D3D12_RESOURCE_FLAG_NONE);

		auto buffer = std::unique_ptr<ConstantBuffer>(new D3D12ConstantBuffer(allocation, elementCount, instanceCount, static_cast<uint32_t>(elementStride)));
		return std::move(buffer);
	}


	std::unique_ptr<ByteAddressBuffer> D3D12ResourceFactory::CreateByteAddressBuffer(uint64_t width, bool readOnly) const
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		if (!readOnly)
		{
			flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}

		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_DEFAULT,
			width,
			flags);

		auto buffer = std::unique_ptr<ByteAddressBuffer>(new D3D12ByteAddressBuffer(allocation, readOnly));
		return std::move(buffer);
	}


	std::unique_ptr<StructuredBuffer> D3D12ResourceFactory::CreateStructuredBuffer(uint32_t elementCount, uint32_t elementSize, bool readOnly) const
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		if (!readOnly)
		{
			flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}

		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_DEFAULT,
			static_cast<uint64_t>(elementCount * elementSize),
			flags);

		auto buffer = std::unique_ptr<StructuredBuffer>(new D3D12StructuredBuffer(allocation, elementCount, elementSize, readOnly));
		return std::move(buffer);
	}


	std::unique_ptr<ReadbackBuffer> D3D12ResourceFactory::CreateReadbackBuffer(uint64_t width) const
	{
		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_READBACK,
			width,
			D3D12_RESOURCE_FLAG_NONE);

		auto buffer = std::unique_ptr<ReadbackBuffer>(new D3D12ReadbackBuffer(allocation));
		return std::move(buffer);
	}


	D3D12MA::Allocation* D3D12ResourceFactory::AllocateBuffer(D3D12_HEAP_TYPE heap, uint64_t width, D3D12_RESOURCE_FLAGS flags) const
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
