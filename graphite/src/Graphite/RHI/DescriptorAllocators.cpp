#include "graphite_pch.h"
#include "DescriptorAllocators.h"

#include "Graphite/Core/Assert.h"
#include "GraphicsContext.h"


namespace Graphite
{
	DescriptorAllocatorInterface::DescriptorAllocatorInterface()
		: m_Heap(nullptr)
		, m_HeapOffset(0)
		, m_Capacity(0)
		, m_CountAllocated(0)
	{
	}

	DescriptorAllocatorInterface::DescriptorAllocatorInterface(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize)
		: m_Heap(heap)
		, m_HeapOffset(rangeStart)
		, m_Capacity(rangeSize)
		, m_CountAllocated(0)
	{
	}


	StaticDescriptorAllocator::StaticDescriptorAllocator(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize)
		: DescriptorAllocatorInterface(heap, rangeStart, rangeSize)
	{
		// Currently, the entire heap is free
		m_FreeBlocks.insert({ 0, m_Capacity });
		m_DeferredFrees.resize(GraphicsContext::GetBackBufferCount());
	}

	StaticDescriptorAllocator::~StaticDescriptorAllocator()
	{
		GRAPHITE_ASSERT(m_CountAllocated == 0, "Cannot destroy allocator before all allocations are freed!");
	}

	DescriptorAllocation StaticDescriptorAllocator::Allocate(uint32_t countToAlloc)
	{
		GRAPHITE_ASSERT(countToAlloc > 0, "Invalid quantity of descriptors");

		if (m_CountAllocated + countToAlloc > m_Capacity)
		{
			GRAPHITE_LOG_ERROR("Descriptor allocation failed: no capacity in heap. Heap size: {0} Count to alloc: {1}", m_Capacity, countToAlloc);
			return {};
		}

		// Find the next free block
		auto freeBlockIt = m_FreeBlocks.begin();
		for (; freeBlockIt != m_FreeBlocks.end() && freeBlockIt->second < countToAlloc; ++freeBlockIt) {}
		if (freeBlockIt == m_FreeBlocks.end())
		{
			// No space in the heap
			GRAPHITE_LOG_ERROR("Descriptor allocation failed: no capacity in heap. Heap size: {0} Count to alloc: {1}", m_Capacity, countToAlloc);
			return {};
		}


		const uint32_t allocIndex = freeBlockIt->first;

		// Update the free blocks to reflect the changes
		uint32_t newFreeIndex = allocIndex + countToAlloc;
		uint32_t newFreeSize = freeBlockIt->second - countToAlloc;
		m_FreeBlocks.erase(freeBlockIt);
		// If there is still empty space insert it into the free blocks
		if (newFreeSize > 0)
		{
			m_FreeBlocks.insert(std::make_pair(newFreeIndex, newFreeSize));
		}

		m_CountAllocated += countToAlloc;

		return DescriptorAllocation{ m_Heap, this, allocIndex, countToAlloc, m_Heap->IsCPUOnly() };
	}

	void StaticDescriptorAllocator::Free(DescriptorAllocation& allocation)
	{
		if (!allocation.IsValid())
			return;

		// Make sure this heap contains this allocation
		GRAPHITE_ASSERT(allocation.GetAllocator() == this, "Trying to free an allocation from the wrong allocator");

		m_DeferredFrees.at(g_GraphicsContext->GetCurrentBackBuffer()).push_back(std::make_pair(allocation.GetIndex(), allocation.GetCount()));
		allocation.ResetWithoutFree();
	}

	void StaticDescriptorAllocator::ReleasePendingFrees(uint32_t frameIndex)
	{
		// TODO: add validation

		if (m_DeferredFrees.at(frameIndex).empty())
			// No deferred frees to be made
			return;

		for (const auto& freedRange : m_DeferredFrees.at(frameIndex))
		{
			// Return freed allocation to the heap
			m_FreeBlocks.insert(freedRange);
			m_CountAllocated -= freedRange.second;
		}
		m_DeferredFrees.at(frameIndex).clear();

		// Combine adjacent freed blocks
		for (auto it = m_FreeBlocks.begin(); it != m_FreeBlocks.end();)
		{
			auto toMerge = m_FreeBlocks.find(it->first + it->second);
			if (toMerge != m_FreeBlocks.end())
			{
				// Contiguous block exists, merge into a single block
				it->second += toMerge->second;
				it = m_FreeBlocks.erase(toMerge);
			}
			else
			{
				++it;
			}
		}
	}


	DynamicDescriptorAllocator::DynamicDescriptorAllocator(DescriptorHeap* heap, uint32_t rangeStart, uint32_t rangeSize, uint32_t frameIndex)
		: DescriptorAllocatorInterface(heap, rangeStart, rangeSize)
		, m_FrameIndex(frameIndex)
	{
	}

	DescriptorAllocation DynamicDescriptorAllocator::Allocate(uint32_t countToAlloc)
	{
		GRAPHITE_ASSERT(countToAlloc > 0, "Invalid quantity of descriptors");

		if (m_CountAllocated + countToAlloc > m_Capacity)
		{
			GRAPHITE_LOG_ERROR("Descriptor allocation failed: no capacity in heap. Heap size: {0} Count to alloc: {1}", m_Capacity, countToAlloc);
			return {};
		}

		uint32_t allocIndex = m_CountAllocated;
		m_CountAllocated += countToAlloc;

		return DescriptorAllocation{ m_Heap, this, allocIndex, countToAlloc, m_Heap->IsCPUOnly() };
	}

	void DynamicDescriptorAllocator::Free(DescriptorAllocation& allocation)
	{
		if (!allocation.IsValid())
			return;

		// Make sure this heap contains this allocation
		GRAPHITE_ASSERT(allocation.GetAllocator() == this, "Trying to free an allocation from the wrong allocator");

		allocation.ResetWithoutFree();
	}

	void DynamicDescriptorAllocator::ReleasePendingFrees(uint32_t frameIndex)
	{
		if (frameIndex != m_FrameIndex)
		{
			return;
		}

		// Descriptors are released simply by allowing them to be overwritten next frame
		m_CountAllocated = 0;
	}
}
