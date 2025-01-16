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
		GRAPHITE_API DescriptorAllocatorInterface();
		GRAPHITE_API DescriptorAllocatorInterface(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize);
		GRAPHITE_API virtual ~DescriptorAllocatorInterface() = default;

		GRAPHITE_API_DELETE_COPY(DescriptorAllocatorInterface);
		GRAPHITE_API_DEFAULT_MOVE(DescriptorAllocatorInterface);

		GRAPHITE_API virtual DescriptorAllocation Allocate(uint32_t countToAlloc) = 0;
		GRAPHITE_API virtual void Free(DescriptorAllocation& allocation) = 0;

		GRAPHITE_API virtual void ReleasePendingFrees(uint32_t frameIndex) = 0;


		GRAPHITE_API inline DescriptorHeap* GetHeap() const { return m_Heap; }

		GRAPHITE_API inline uint32_t GetCapacity() const { return m_Capacity; }
		GRAPHITE_API inline uint32_t GetCountAllocated() const { return m_CountAllocated; }

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
		GRAPHITE_API StaticDescriptorAllocator() = default;
		GRAPHITE_API StaticDescriptorAllocator(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize);
		GRAPHITE_API virtual ~StaticDescriptorAllocator();

		GRAPHITE_API_DELETE_COPY(StaticDescriptorAllocator);
		GRAPHITE_API_DEFAULT_MOVE(StaticDescriptorAllocator);

		GRAPHITE_API virtual DescriptorAllocation Allocate(uint32_t countToAlloc) override;
		GRAPHITE_API virtual void Free(DescriptorAllocation& allocation) override;

		GRAPHITE_API virtual void ReleasePendingFrees(uint32_t frameIndex) override;

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
		GRAPHITE_API DynamicDescriptorAllocator() = default;
		// FrameIndex specifies which frame this allocator is allocating for.
		// This is to allow this allocator to know when it's allocations will no longer be in flight
		GRAPHITE_API DynamicDescriptorAllocator(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize, uint32_t frameIndex);
		GRAPHITE_API virtual ~DynamicDescriptorAllocator() = default;

		GRAPHITE_API_DELETE_COPY(DynamicDescriptorAllocator);
		GRAPHITE_API_DEFAULT_MOVE(DynamicDescriptorAllocator);

		GRAPHITE_API virtual DescriptorAllocation Allocate(uint32_t countToAlloc) override;
		GRAPHITE_API virtual void Free(DescriptorAllocation& allocation) override;

		GRAPHITE_API virtual void ReleasePendingFrees(uint32_t frameIndex) override;
	protected:
		uint32_t m_FrameIndex;
	};

}
