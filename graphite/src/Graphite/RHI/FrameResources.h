#pragma once

#include "Graphite/Core/Core.h"

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

		void Init(ID3D12Device* device, uint32_t frameResourcesIndex);

		// Getters
		inline ID3D12CommandAllocator* GetCommandAllocator() const { return m_CommandAllocator.Get(); }
		inline UINT64 GetFenceValue() const { return m_FenceValue; }

		// Reset alloc
		void ResetAllocator() const;

		// Fence
		inline void IncrementFence() { m_FenceValue++; }
		inline void SetFence(UINT64 fence) { m_FenceValue = fence; }

	private:
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		// Update the fence value for the frame resources whenever work is submitted in a frame
		// - then when you wait on the fence value for those frame resources, all work that was submitted in that frame will be complete
		UINT64 m_FenceValue = 0;
	};
}
