#pragma once

#include "Graphite/Core/Core.h"

#include "RHITypes.h"


namespace Graphite
{
	class DescriptorHeap;
	class DescriptorAllocatorInterface;


	class DescriptorAllocation
	{
	public:
		GRAPHITE_API DescriptorAllocation() = default;
		GRAPHITE_API DescriptorAllocation(
			DescriptorHeap* heap,
			DescriptorAllocatorInterface* allocator,
			uint32_t index,
			uint32_t count,
			bool cpuOnly
		);
		GRAPHITE_API ~DescriptorAllocation();

		GRAPHITE_API_DELETE_COPY(DescriptorAllocation);
		// Moving from a descriptor requires invalidating the moved-from descriptor
		GRAPHITE_API DescriptorAllocation(DescriptorAllocation&& other) noexcept;
		GRAPHITE_API DescriptorAllocation& operator=(DescriptorAllocation&& other) noexcept;

		GRAPHITE_API CPUDescriptorHandle GetCPUHandle(uint32_t index = 0) const;
		GRAPHITE_API GPUDescriptorHandle GetGPUHandle(uint32_t index = 0) const;

		GRAPHITE_API void ResetWithoutFree();
		GRAPHITE_API void Free();

		GRAPHITE_API inline bool IsValid() const { return m_IsValid; }
		GRAPHITE_API inline bool IsCPUOnly() const { return m_CPUOnly; }

		GRAPHITE_API DescriptorHeap* GetHeap() const;
		GRAPHITE_API DescriptorAllocatorInterface* GetAllocator() const;

		GRAPHITE_API inline uint32_t GetIndex() const { return m_Index; }
		GRAPHITE_API inline uint32_t GetCount() const { return m_Count; }

	private:
		DescriptorHeap* m_Heap = nullptr;						// The heap that this allocation belongs to
		DescriptorAllocatorInterface* m_Allocator = nullptr;	// The allocator that made this allocation

		uint32_t m_Index = 0;									// The index this allocation begins at
		uint32_t m_Count = 0;									// The number of descriptors in this allocation

		bool m_CPUOnly = false;									// Is this heap CPU only

		bool m_IsValid = false;									// Does this allocation actually point to a real allocated descriptor
	};


	class DescriptorHeap
	{
	public:
		GRAPHITE_API DescriptorHeap(DescriptorHeapType heapType, uint32_t descriptorSize, uint32_t capacity, bool cpuOnly);
		GRAPHITE_API virtual ~DescriptorHeap() = default;

		// cannot be copied or moved
		DELETE_COPY(DescriptorHeap);
		DEFAULT_MOVE(DescriptorHeap);

		GRAPHITE_API virtual CPUDescriptorHandle GetCPUHandleForHeapStart() const = 0;
		GRAPHITE_API virtual GPUDescriptorHandle GetGPUHandleForHeapStart() const = 0;

		// Getters
		GRAPHITE_API inline DescriptorHeapType GetType() const { return m_HeapType; }
		GRAPHITE_API inline uint32_t GetDescriptorSize() const { return m_DescriptorSize; }

		GRAPHITE_API inline uint32_t GetCapacity() const { return m_Capacity; }

		GRAPHITE_API inline bool IsCPUOnly() const { return m_CPUOnly; }

	protected:
		DescriptorHeapType m_HeapType;
		uint32_t m_DescriptorSize;

		uint32_t m_Capacity;

		bool m_CPUOnly;
	};
}
