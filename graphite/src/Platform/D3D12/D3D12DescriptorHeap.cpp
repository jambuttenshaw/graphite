#include "graphite_d3d12_pch.h"
#include "D3D12DescriptorHeap.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/GraphicsContext.h"

#include "D3D12Types.h"
#include "Platform/D3D12/D3D12Exceptions.h"


namespace Graphite::D3D12
{

	D3D12DescriptorHeap::D3D12DescriptorHeap(ID3D12Device* device, DescriptorHeapType type, uint32_t capacity, bool cpuOnly, const wchar_t* name)
		: DescriptorHeap(
			type,
			0,	// Descriptor size will be initialized later - device must be queries first
			capacity,
			cpuOnly
		)
	{
		m_NativeHeapType = GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(type);

		// RTV and DSV heaps cannot be gpu visible
		if (m_NativeHeapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || m_NativeHeapType == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
			m_CPUOnly = true;

		// TODO: Add assertions for capacity/type combos (sampler heaps have smaller max size)

		// Create the descriptor heap
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = m_NativeHeapType;
		desc.NumDescriptors = m_Capacity;
		desc.Flags = m_CPUOnly ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 1;

		DX_THROW_IF_FAIL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_NativeHeap)));
		if (name)
		{
			DX_THROW_IF_FAIL(m_NativeHeap->SetName(name));
		}

		m_DescriptorSize = device->GetDescriptorHandleIncrementSize(m_NativeHeapType);

		// Currently, the entire heap is free
		m_FreeBlocks.insert({ 0, m_Capacity });
		m_DeferredFrees.resize(GraphicsContext::GetBackBufferCount());
	}

	D3D12DescriptorHeap::~D3D12DescriptorHeap()
	{
		GRAPHITE_ASSERT(m_CountAllocated == 0, "Don't destroy allocators that still have memory allocated!");

		// Heap will automatically be released
		// GPU must not be using any of the allocations made

		// TODO: defer release of the heap (to make sure GPU is idle when this is released)
	}

	DescriptorAllocation D3D12DescriptorHeap::Allocate(uint32_t countToAlloc)
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

		return DescriptorAllocation{ this, allocIndex, countToAlloc, m_CPUOnly };
	}

	void D3D12DescriptorHeap::Free(DescriptorAllocation& allocation)
	{
		if (!allocation.IsValid())
			return;

		// Make sure this heap contains this allocation
		GRAPHITE_ASSERT(allocation.GetHeap() == this, "Trying to free an allocation from the wrong heap");

		m_DeferredFrees[m_CurrentDeferredFreeCollection].push_back(std::make_pair(allocation.GetIndex(), allocation.GetCount()));
		allocation.ResetWithoutFree();
	}

	CPUDescriptorHandle D3D12DescriptorHeap::GetCPUHandleForHeapStart() const
	{
		return D3D12CPUDescriptorToGraphiteDescriptor(m_NativeHeap->GetCPUDescriptorHandleForHeapStart());
	}

	GPUDescriptorHandle D3D12DescriptorHeap::GetGPUHandleForHeapStart() const
	{
		return D3D12GPUDescriptorToGraphiteDescriptor(m_NativeHeap->GetGPUDescriptorHandleForHeapStart());
	}

	void D3D12DescriptorHeap::ProcessDeferredFrees(uint32_t frameIndex)
	{
		// TODO: add validation

		if (m_DeferredFrees[frameIndex].empty())
			// No deferred frees to be made
			return;

		for (const auto& freedRange : m_DeferredFrees[frameIndex])
		{
			// Return freed allocation to the heap
			m_FreeBlocks.insert(freedRange);
			m_CountAllocated -= freedRange.second;
		}
		m_DeferredFrees[frameIndex].clear();

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

		m_CurrentDeferredFreeCollection = (m_CurrentDeferredFreeCollection + 1) % m_DeferredFrees.size();
	}
}