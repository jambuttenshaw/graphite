#include "pch.h"
#include "ResourceManager.h"

#include "D3D12MemAlloc.h"
#include "RHIExceptions.h"
#include "Graphite/Core/Assert.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{
	ResourceManager& ResourceManager::Get()
	{
		static ResourceManager instance;
		return instance;
	}


	void ResourceManager::CreateAllocator(IDXGIAdapter* adapter, ID3D12Device* device)
	{
		D3D12MA::ALLOCATOR_DESC desc = {};
		desc.pDevice = device;
		desc.pAdapter = adapter;
		desc.Flags = D3D12MA::ALLOCATOR_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED | D3D12MA::ALLOCATOR_FLAG_DEFAULT_POOLS_NOT_ZEROED;

		DX_THROW_IF_FAIL(D3D12MA::CreateAllocator(&desc, &m_Allocator));
	}

	void ResourceManager::DestroyAllocator()
	{
		GRAPHITE_ASSERT(m_Allocator, "Cannot destroy allocator - CreateAllocator was never called!");
		m_Allocator->Release();
	}

}
