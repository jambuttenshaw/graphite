#pragma once

#include "Graphite/Core/Core.h"

#include "RHITypes.h"


namespace Graphite
{
	class DescriptorHeap;


	class DescriptorAllocation
	{
	public:
		DescriptorAllocation() = default;
		DescriptorAllocation(
			DescriptorHeap* heap,
			uint32_t index,
			uint32_t count,
			bool cpuOnly
		);
		~DescriptorAllocation();

		DELETE_COPY(DescriptorAllocation);
		// Moving from a descriptor requires invalidating the moved-from descriptor
		DescriptorAllocation(DescriptorAllocation&& other) noexcept;			
		DescriptorAllocation& operator=(DescriptorAllocation&& other) noexcept;

		CPUDescriptorHandle GetCPUHandle(uint32_t index = 0) const;
		GPUDescriptorHandle GetGPUHandle(uint32_t index = 0) const;

		void ResetWithoutFree();
		void Free();

		inline bool IsValid() const { return m_IsValid; }

		DescriptorHeap* GetHeap() const;
		inline uint32_t GetIndex() const { return m_Index; }
		inline uint32_t GetCount() const { return m_Count; }

	private:
		DescriptorHeap* m_Heap = nullptr;	// The allocator that made this allocation
		uint32_t m_Index = 0;				// The index this allocation begins at
		uint32_t m_Count = 0;				// The number of descriptors in this allocation

		bool m_CPUOnly = false;				// Is this heap CPU only

		bool m_IsValid = false;				// Does this allocation actually point to a real allocated descriptor
	};


	class DescriptorHeap
	{
	public:
		DescriptorHeap(DescriptorHeapType heapType, uint32_t descriptorSize, uint32_t capacity, bool cpuOnly);
		virtual ~DescriptorHeap() = default;

		// cannot be copied or moved
		DELETE_COPY(DescriptorHeap);
		DEFAULT_MOVE(DescriptorHeap);

		virtual DescriptorAllocation Allocate(uint32_t countToAlloc) = 0;
		virtual void Free(DescriptorAllocation& allocation) = 0;

		virtual CPUDescriptorHandle GetCPUHandleForHeapStart() const = 0;
		virtual GPUDescriptorHandle GetGPUHandleForHeapStart() const = 0;

		// Getters
		inline DescriptorHeapType GetType() const { return m_HeapType; }
		inline uint32_t GetDescriptorSize() const { return m_DescriptorSize; }

		inline uint32_t GetCapacity() const { return m_Capacity; }
		inline uint32_t GetCountAllocated() const { return m_CountAllocated; }

	protected:
		DescriptorHeapType m_HeapType;
		uint32_t m_DescriptorSize;

		uint32_t m_Capacity;
		uint32_t m_CountAllocated;

		bool m_CPUOnly;
	};

}
