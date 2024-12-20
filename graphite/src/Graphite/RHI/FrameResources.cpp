#include "pch.h"
#include "FrameResources.h"

#include "RHIExceptions.h"
#include "Graphite/Core/Assert.h"


namespace Graphite
{
	FrameResources::FrameResources()
	{
		
	}

	FrameResources::~FrameResources()
	{
		
	}

	void FrameResources::Init(ID3D12Device* device, uint32_t frameResourcesIndex, uint32_t allocatorPoolSize)
	{
		// Create command allocators
		m_AllocatorPool.resize(allocatorPoolSize);

		for (size_t i = 0; i < m_AllocatorPool.size(); i++)
		{
			auto& allocator = m_AllocatorPool.at(i);

			DX_THROW_IF_FAIL(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
	#ifdef GRAPHITE_DEBUG
			{
				std::wstring resourcesName{ L"Frame Resources " };
				resourcesName += std::to_wstring(frameResourcesIndex) + L", Allocator " + std::to_wstring(i);
				allocator->SetName(resourcesName.c_str());
			}
	#endif
		}
	}

	ID3D12CommandAllocator* FrameResources::GetNextCommandAllocator()
	{
		GRAPHITE_ASSERT(m_ActiveAllocators < GetAllocatorPoolSize(), "Allocator pool exceeded!");
		return m_AllocatorPool.at(m_ActiveAllocators++).Get();
	}

	void FrameResources::ResetAllAllocators()
	{
		for (auto& allocator : m_AllocatorPool)
		{
			DX_THROW_IF_FAIL(allocator->Reset());
		}
		m_ActiveAllocators = 0;
	}



}
