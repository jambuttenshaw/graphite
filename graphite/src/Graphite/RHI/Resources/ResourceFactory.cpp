#include "pch.h"
#include "ResourceFactory.h"

#include "Graphite/RHI/RHIExceptions.h"
#include "Graphite/Core/Assert.h"

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


	uint64_t ResourceFactory::AlignSize(uint64_t size, uint64_t alignment)
	{
		return (size + (alignment - 1)) & ~(alignment - 1);
	}


}
