#include "graphite_d3d12_pch.h"
#include "D3D12ResourceFactory.h"

#include "D3D12MemAlloc.h"

#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/Resources/ResourceFactory.h"

#include "RHI/D3D12GraphicsContext.h"
#include "RHI/D3D12Exceptions.h"

#include "D3D12Buffer.h"
#include "D3D12Texture.h"
#include "D3D12VertexBuffer.h"

#include "Graphite/RHI/Resources/InputLayout.h"
#include "RHI/D3D12Types.h"


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
			elementStride = AlignSize(elementStride, elementAlignment);
		}
		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_UPLOAD,
			elementStride * elementCount * instanceCount,
			D3D12_RESOURCE_FLAG_NONE);

		auto buffer = std::unique_ptr<UploadBuffer>(new D3D12UploadBuffer(allocation, elementCount, instanceCount, static_cast<uint32_t>(elementStride)));
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


	std::unique_ptr<VertexBuffer> D3D12ResourceFactory::CreateVertexBuffer(uint32_t vertexCount, const InputLayout& inputLayout) const
	{
		const uint64_t width = static_cast<uint64_t>(vertexCount) * inputLayout.GetLayoutSizeInBytes();
		auto allocation = AllocateBuffer(D3D12_HEAP_TYPE_UPLOAD, width, D3D12_RESOURCE_FLAG_NONE);

		auto buffer = std::unique_ptr<VertexBuffer>(new D3D12VertexBuffer(allocation, vertexCount, inputLayout));
		return std::move(buffer);
	}


	std::unique_ptr<Texture2D> D3D12ResourceFactory::CreateTexture2D(const Texture2DDesc& desc) const
	{
		bool committedResource = false;
		D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE;
		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = ToD3D12Format(desc.Format);

		if (GPUResource::CheckAccessFlags(desc.AccessFlags, ResourceAccess_GPUWrite))
		{
			resourceFlags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		}
		if (GPUResource::CheckAccessFlags(desc.AccessFlags, ResourceAccess_RenderTarget))
		{
			committedResource = true;
			resourceFlags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

			GRAPHITE_ASSERT(desc.ClearValue.has_value(), "Render targets must be provided with a clear value.");
			clearValue.Color[0] = desc.ClearValue->Color.r;
			clearValue.Color[1] = desc.ClearValue->Color.g;
			clearValue.Color[2] = desc.ClearValue->Color.b;
			clearValue.Color[3] = desc.ClearValue->Color.a;
		}
		if (GPUResource::CheckAccessFlags(desc.AccessFlags, ResourceAccess_DepthStencil))
		{
			committedResource = true;
			resourceFlags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			GRAPHITE_ASSERT(desc.ClearValue.has_value(), "Depth stencil targets must be provided with a clear value.");
			clearValue.DepthStencil.Depth = desc.ClearValue->Depth;
			clearValue.DepthStencil.Stencil = desc.ClearValue->Stencil;
		}

		const auto resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			ToD3D12Format(desc.Format),
			static_cast<UINT64>(desc.Width),
			static_cast<UINT>(desc.Height),
			1,
			0,
			1,
			0,
			resourceFlags
		);

		D3D12MA::ALLOCATION_DESC allocDesc = {};
		allocDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
		allocDesc.Flags |= committedResource ? D3D12MA::ALLOCATION_FLAG_COMMITTED : D3D12MA::ALLOCATION_FLAG_NONE;

		D3D12MA::Allocation* allocation;
		DX_THROW_IF_FAIL(m_Allocator->CreateResource(
			&allocDesc,
			&resourceDesc,
			ToD3D12ResourceState(desc.InitialState),
			desc.ClearValue.has_value() ? &clearValue : nullptr,
			&allocation,
			IID_NULL, nullptr));

		auto texture = std::unique_ptr<Texture2D>(new D3D12Texture2D(allocation, desc));
		return std::move(texture);
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
