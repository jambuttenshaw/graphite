#pragma once

#include "Graphite/Core/Core.h"
#include "Descriptors.h"


namespace Graphite
{

	// Multiple allocation schemes may be desired for the same heap
	// The resulting allocation shouldn't care about how it was allocated
	// Therefore a common interface to the allocator is required
	class DescriptorAllocatorInterface
	{
	public:
		DescriptorAllocatorInterface();
		DescriptorAllocatorInterface(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize);
		virtual ~DescriptorAllocatorInterface() = default;

		DELETE_COPY(DescriptorAllocatorInterface);
		DEFAULT_MOVE(DescriptorAllocatorInterface);

		virtual DescriptorAllocation Allocate(uint32_t countToAlloc) = 0;
		virtual void Free(DescriptorAllocation& allocation) = 0;

		virtual void ReleasePendingFrees(uint32_t frameIndex) = 0;


		inline DescriptorHeap* GetHeap() const { return m_Heap; }

		inline uint32_t GetCapacity() const { return m_Capacity; }
		inline uint32_t GetCountAllocated() const { return m_CountAllocated; }

	protected:
		DescriptorHeap* m_Heap;

		// Describes the subset of the descriptor heap that this allocator can allocate from
		uint32_t m_HeapOffset;
		uint32_t m_Capacity;

		uint32_t m_CountAllocated;
	};


	// Free-list heap allocator for descriptors with long lifetimes
	class StaticDescriptorAllocator : public DescriptorAllocatorInterface
	{
	public:
		StaticDescriptorAllocator() = default;
		StaticDescriptorAllocator(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize);
		virtual ~StaticDescriptorAllocator();

		DELETE_COPY(StaticDescriptorAllocator);
		DEFAULT_MOVE(StaticDescriptorAllocator);

		virtual DescriptorAllocation Allocate(uint32_t countToAlloc) override;
		virtual void Free(DescriptorAllocation& allocation) override;

		virtual void ReleasePendingFrees(uint32_t frameIndex) override;

	protected:
		// Free-list allocator implementation
		std::map<uint32_t, uint32_t> m_FreeBlocks; // Map of index to size of free blocks

		// This is designed with the model that a separate allocator is used per frame in flight.
		// Therefore, once this allocator's frame in flight has completed, all deferred frees can be released
		using PendingFreeCollection = std::vector<std::pair<uint32_t, uint32_t>>;
		std::vector<PendingFreeCollection> m_DeferredFrees;
	};


	// Stack-based descriptor allocator
	// Descriptors should only be used for a SINGLE FRAME
	class DynamicDescriptorAllocator : public DescriptorAllocatorInterface
	{
	public:
		DynamicDescriptorAllocator() = default;
		// FrameIndex specifies which frame this allocator is allocating for.
		// This is to allow this allocator to know when it's allocations will no longer be in flight
		DynamicDescriptorAllocator(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize, uint32_t frameIndex);
		virtual ~DynamicDescriptorAllocator() = default;

		DELETE_COPY(DynamicDescriptorAllocator);
		DEFAULT_MOVE(DynamicDescriptorAllocator);

		virtual DescriptorAllocation Allocate(uint32_t countToAlloc) override;
		virtual void Free(DescriptorAllocation& allocation) override;

		virtual void ReleasePendingFrees(uint32_t frameIndex) override;
	protected:
		uint32_t m_FrameIndex;
	};

}
