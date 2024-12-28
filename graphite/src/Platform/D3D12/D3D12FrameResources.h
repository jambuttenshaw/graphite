#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/Core/Log.h"

using Microsoft::WRL::ComPtr;


namespace Graphite::D3D12
{

	// A collection of resources and state pertinent to each back buffer
	// An array of these will be owned by the back buffer
	class D3D12FrameResources
	{
	public:
		D3D12FrameResources() = default;
		~D3D12FrameResources() = default;

		DELETE_COPY(D3D12FrameResources)
		DEFAULT_MOVE(D3D12FrameResources)

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

		void DeferRelease(const ComPtr<IUnknown>& resource);
		void ProcessDeferrals();

	private:
		// A pool of allocators is used to facilitate multithreaded command recording
		std::vector<ComPtr<ID3D12CommandAllocator>> m_AllocatorPool;
		UINT m_ActiveAllocators = 0;

		// Update the fence value for the frame resources whenever work is submitted in a frame
		// - then when you wait on the fence value for those frame resources, all work that was submitted in that frame will be complete
		UINT64 m_FenceValue = 0;

		// Resources to be released when the GPU is finished with them
		// A release collection is required per frame resources to ensure that
		// the GPU is not currently using any of the resources
		std::vector<ComPtr<IUnknown>> m_DeferredReleases;
	};
}
