#include "pch.h"
#include "CommandRecordingContext.h"

#include "RHIExceptions.h"
#include "Graphite/Core/Assert.h"


namespace Graphite
{
	CommandRecordingContext::CommandRecordingContext(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name)
	{
		// Create command list
		DX_THROW_IF_FAIL(device->CreateCommandList(0, type, allocator, nullptr, IID_PPV_ARGS(m_CommandList.ReleaseAndGetAddressOf())));
		m_IsClosed = false;

#ifdef GRAPHITE_DEBUG
		if (name)
		{
			DX_THROW_IF_FAIL(m_CommandList->SetName(name));
		}
#endif
	}

	void CommandRecordingContext::Reset(ID3D12CommandAllocator* allocator)
	{
		DX_THROW_IF_FAIL(m_CommandList->Reset(allocator, nullptr));
		m_IsClosed = false;
	}

	void CommandRecordingContext::Close()
	{
		DX_THROW_IF_FAIL(m_CommandList->Close());
		m_IsClosed = true;
	}


	// Command implementations

	void CommandRecordingContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView, float* clearColor) const
	{
		GRAPHITE_ASSERT(!m_IsClosed, "Cannot add commands to a closed context!");
		m_CommandList->ClearRenderTargetView(renderTargetView, clearColor, 0, nullptr);
	}

}
