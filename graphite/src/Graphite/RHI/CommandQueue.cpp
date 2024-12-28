#include "graphite_pch.h"
#include "CommandQueue.h"

#include "RHIExceptions.h"
#include "Graphite/Core/Assert.h"

namespace Graphite
{

	CommandQueue::CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name)
		: m_QueueType(type)
	{
		// Create queue
		{
			D3D12_COMMAND_QUEUE_DESC desc;
			desc.Type = m_QueueType;
			desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 0;
			DX_THROW_IF_FAIL(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue)));

#ifdef GRAPHITE_DEBUG
			if (name)
			{
				m_CommandQueue->SetName(name);
			}
#endif
		}

		// Assign initial fence values
		m_LastCompletedFenceValue = 0;
		m_NextFenceValue = 1;

		// Create fence
		{
			DX_THROW_IF_FAIL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
			DX_THROW_IF_FAIL(m_Fence->Signal(m_LastCompletedFenceValue));

#ifdef GRAPHITE_DEBUG
			if (name)
			{
				std::wstring fenceName(name);
				fenceName += L" Fence";
				m_Fence->SetName(fenceName.c_str());
			}
#endif
		}

		// Create fence event
		{
			m_FenceEventHandle = CreateEventEx(nullptr, L"Queue Fence Event", 0, EVENT_ALL_ACCESS);
			GRAPHITE_ASSERT(m_FenceEventHandle != INVALID_HANDLE_VALUE, "Failed to create fence event.");
		}
	}

	CommandQueue::~CommandQueue()
	{
		CloseHandle(m_FenceEventHandle);
	}

	UINT64 CommandQueue::ExecuteCommandLists(UINT count, ID3D12CommandList** ppCommandLists)
	{
		m_CommandQueue->ExecuteCommandLists(count, ppCommandLists);
		return Signal();
	}

	UINT64 CommandQueue::Signal()
	{
		std::lock_guard lock(m_FenceMutex);
		m_CommandQueue->Signal(m_Fence.Get(), m_NextFenceValue);

		return m_NextFenceValue++;
	}

	void CommandQueue::InsertWait(UINT64 fenceValue) const
	{
		DX_THROW_IF_FAIL(m_CommandQueue->Wait(m_Fence.Get(), fenceValue));
	}

	void CommandQueue::InsertWaitForQueueFence(const CommandQueue* otherQueue, UINT64 fenceValue) const
	{
		GRAPHITE_ASSERT(otherQueue, "Null queue");
		DX_THROW_IF_FAIL(m_CommandQueue->Wait(otherQueue->GetFence(), fenceValue));

	}

	void CommandQueue::InsertWaitForQueue(const CommandQueue* otherQueue) const
	{
		GRAPHITE_ASSERT(otherQueue, "Null queue");
		DX_THROW_IF_FAIL(m_CommandQueue->Wait(otherQueue->GetFence(), otherQueue->GetNextFenceValue() - 1));
	}

	void CommandQueue::WaitForFenceCPUBlocking(UINT64 fenceValue)
	{
		while (!IsFenceComplete(fenceValue))
		{
			std::lock_guard lockGuard(m_EventMutex);

			m_Fence->SetEventOnCompletion(fenceValue, m_FenceEventHandle);
			// When it wakes up - it will check that the expected fence value has actually passed
			// This handles spurious wake-ups, and also cases where we accidentally sleep after the fence has passed
			WaitForSingleObjectEx(m_FenceEventHandle, 1, false);
		}
		m_LastCompletedFenceValue = fenceValue;
	}

	void CommandQueue::WaitForIdleCPUBlocking()
	{
		// Add an additional signal so that when the signaled value is reached we know that all work prior to this call has been completed
		WaitForFenceCPUBlocking(Signal());
	}


	bool CommandQueue::IsFenceComplete(UINT64 fenceValue)
	{
		if (fenceValue > m_LastCompletedFenceValue)
		{
			PollCurrentFenceValue();
		}

		return fenceValue <= m_LastCompletedFenceValue;
	}

	UINT64 CommandQueue::PollCurrentFenceValue()
	{
		const auto fenceValue = m_Fence->GetCompletedValue();
		if (fenceValue > m_LastCompletedFenceValue)
		{
			m_LastCompletedFenceValue = fenceValue;
		}

		return m_LastCompletedFenceValue;
	}
}
