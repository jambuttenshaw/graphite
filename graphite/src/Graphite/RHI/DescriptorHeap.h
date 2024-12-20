#pragma once

#include "Graphite/Core/Core.h"

using Microsoft::WRL::ComPtr;


namespace Graphite
{

	class DescriptorHeap;


	class DescriptorAllocation
	{
	public:
		DescriptorAllocation() = default;
		DescriptorAllocation(
			DescriptorHeap* heap,
			UINT index,
			UINT count,
			bool cpuOnly
		);
		~DescriptorAllocation();

		DELETE_COPY(DescriptorAllocation)
		DescriptorAllocation(DescriptorAllocation&& other) noexcept;
		DescriptorAllocation& operator=(DescriptorAllocation&& other) noexcept;

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UINT index = 0) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(UINT index = 0) const;

		void Reset();
		void Free();

		inline bool IsValid() const { return m_IsValid; }

		ID3D12DescriptorHeap* GetHeap() const;
		inline UINT GetIndex() const { return m_Index; }
		inline UINT GetCount() const { return m_Count; }

	private:
		DescriptorHeap* m_Heap = nullptr;	// The allocator that made this allocation
		UINT m_Index = 0;					// The index this allocation begins at
		UINT m_Count = 0;					// The number of descriptors in this allocation

		bool m_CPUOnly = false;				// Is this heap CPU only

		bool m_IsValid = false;				// Does this allocation actually point to a real allocated descriptor
	};


	class DescriptorHeap
	{
	public:
		DescriptorHeap() = default;
		~DescriptorHeap();

		// cannot be copied or moved
		DELETE_COPY(DescriptorHeap);
		DEFAULT_MOVE(DescriptorHeap);

		void Init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT capacity, bool cpuOnly, const wchar_t* name);

		DescriptorAllocation Allocate(UINT countToAlloc);
		void Free(DescriptorAllocation& allocation);

		void ProcessDeferredFree(UINT frameIndex);

		// Getters
		inline ID3D12DescriptorHeap* GetHeap() const { return m_Heap.Get(); }
		inline D3D12_DESCRIPTOR_HEAP_TYPE GetType() const { return m_Type; }
		inline UINT GetDescriptorSize() const { return m_DescriptorIncrementSize; }

		inline UINT GetCapacity() const { return m_Capacity; }
		inline UINT GetCount() const { return m_Count; }

	private:
		// In this design, a D3DDescriptor heap owns its entire heap
		ComPtr<ID3D12DescriptorHeap> m_Heap;	// The heap to allocate from

		D3D12_DESCRIPTOR_HEAP_TYPE m_Type;		// The type of descriptors heap contains
		UINT m_DescriptorIncrementSize = 0;

		UINT m_Capacity = 0;
		UINT m_Count = 0;

		bool m_CPUOnly = false;

		std::map<UINT, UINT> m_FreeBlocks; // Map of index to size of free blocks

		using DeferredFreeCollection = std::vector<std::pair<UINT, UINT>>;
		std::vector<DeferredFreeCollection> m_DeferredFrees; // One collection is required for each frame in flight
		UINT m_CurrentDeferredFreeCollection = 0;
	};
}