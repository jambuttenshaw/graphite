#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/Core/Log.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{

	// A collection of resources and state pertinent to each back buffer
	// An array of these will be owned by the back buffer
	class FrameResources
	{
	public:
		FrameResources();
		~FrameResources();

		DELETE_COPY(FrameResources)
		DEFAULT_MOVE(FrameResources)

		void Init(ID3D12Device* device, uint32_t frameResourcesIndex, uint32_t allocatorPoolSize);

		// Allocators
		size_t GetAllocatorPoolSize() const { return m_AllocatorPool.size(); }

		ID3D12CommandAllocator* GetNextCommandAllocator();
		void ResetAllAllocators();

		// Fence
		inline UINT64 GetFenceValue() const { return m_FenceValue; }

		inline void IncrementFence() { m_FenceValue++; }
		inline void SetFence(UINT64 fence)
		{
#ifdef GRAPHITE_DEBUG
			if (fence <= m_FenceValue)
			{
				GRAPHITE_LOG_WARN("New fence value did not increase.");
			}
#endif
			m_FenceValue = fence;
		}

	private:
		// A pool of allocators is used to facilitate multithreaded command recording
		std::vector<ComPtr<ID3D12CommandAllocator>> m_AllocatorPool;
		UINT m_ActiveAllocators = 0;

		// Update the fence value for the frame resources whenever work is submitted in a frame
		// - then when you wait on the fence value for those frame resources, all work that was submitted in that frame will be complete
		UINT64 m_FenceValue = 0;
	};
}
