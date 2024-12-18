#pragma once

#include "Graphite/Core/Core.h"

using Microsoft::WRL::ComPtr;

namespace Graphite
{

	class CommandQueue
	{
	public:
		CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, const wchar_t* name = nullptr);
		~CommandQueue();

		DELETE_COPY(CommandQueue)
			DEFAULT_MOVE(CommandQueue)

			// Execute work
			UINT64 ExecuteCommandLists(UINT count, ID3D12CommandList** ppCommandLists);

		// GPU wait
		void InsertWait(UINT64 fenceValue) const;
		void InsertWaitForQueueFence(const CommandQueue* otherQueue, UINT64 fenceValue) const;
		void InsertWaitForQueue(const CommandQueue* otherQueue) const;

		// CPU wait
		void WaitForFenceCPUBlocking(UINT64 fenceValue);
		void WaitForIdleCPUBlocking();

		// Polling
		bool IsFenceComplete(UINT64 fenceValue);
		UINT64 PollCurrentFenceValue();
		inline UINT64 GetLastCompletedFence() const { return m_LastCompletedFenceValue; }
		inline UINT64 GetNextFenceValue() const { return m_NextFenceValue; }

		// Resources
		inline ID3D12CommandQueue* GetRHIQueue() const { return m_CommandQueue.Get(); }
		inline ID3D12Fence* GetFence() const { return m_Fence.Get(); }

	private:
		UINT64 Signal();

	private:
		// D3D12 resources
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12Fence> m_Fence;

		D3D12_COMMAND_LIST_TYPE m_QueueType;

		// Mutexes
		std::mutex m_EventMutex;
		std::mutex m_FenceMutex;

		UINT64 m_NextFenceValue = static_cast<UINT64>(-1);
		UINT64 m_LastCompletedFenceValue = static_cast<UINT64>(-1);

		HANDLE m_FenceEventHandle = nullptr;
	};
}