#include "pch.h"
#include "FrameResources.h"

#include "RHIExceptions.h"


namespace Graphite
{
	FrameResources::FrameResources()
	{
		
	}

	FrameResources::~FrameResources()
	{
		
	}

	void FrameResources::Init(ID3D12Device* device, uint32_t frameResourcesIndex)
	{
		// Create command allocator
		DX_THROW_IF_FAIL(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
#ifdef GRAPHITE_DEBUG
		{
			std::wstring resourcesName{ L"Frame Resources " };
			resourcesName += std::to_wstring(frameResourcesIndex);
			m_CommandAllocator->SetName(resourcesName.c_str());
		}
#endif
	}

	void FrameResources::ResetAllocator() const
	{
		DX_THROW_IF_FAIL(m_CommandAllocator->Reset());
	}



}
