#include "graphite_pch.h"
#include "D3D12DescriptorHeap.h"

#include "Graphite/Core/Assert.h"
#include "Platform/D3D12/D3D12Exceptions.h"
#include "Graphite/RHI/GraphicsContext.h"


namespace Graphite::D3D12
{
	// DescriptorAllocation


	D3D12DescriptorAllocation::D3D12DescriptorAllocation(D3D12DescriptorHeap* heap, uint32_t index, uint32_t count, bool cpuOnly)
		: m_Heap(heap)
		, m_Index(index)
		, m_Count(count)
		, m_CPUOnly(cpuOnly)
		, m_IsValid(true)
	{
	}

	D3D12DescriptorAllocation::~D3D12DescriptorAllocation()
	{
		if (IsValid())
		{
			Free();
		}
	}


	D3D12DescriptorAllocation::D3D12DescriptorAllocation(D3D12DescriptorAllocation&& other) noexcept
	{
		m_Heap = std::move(other.m_Heap);
		m_Index = std::move(other.m_Index);
		m_Count = std::move(other.m_Count);

		m_CPUOnly = std::move(other.m_CPUOnly);

		m_IsValid = std::move(other.m_IsValid);

		other.Reset();
	}

	D3D12DescriptorAllocation& D3D12DescriptorAllocation::operator=(D3D12DescriptorAllocation&& other) noexcept
	{
		m_Heap = std::move(other.m_Heap);
		m_Index = std::move(other.m_Index);
		m_Count = std::move(other.m_Count);

		m_CPUOnly = std::move(other.m_CPUOnly);

		m_IsValid = std::move(other.m_IsValid);

		other.Reset();

		return *this;
	}


	D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorAllocation::GetCPUHandle(uint32_t index) const
	{
		GRAPHITE_ASSERT(m_IsValid, "Cannot get handle on an invalid allocation");
		GRAPHITE_ASSERT(index < m_Count, "Invalid descriptor index");

		const INT offset = static_cast<INT>(m_Index + index);
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_Heap->GetHeap()->GetCPUDescriptorHandleForHeapStart(), offset, m_Heap->GetDescriptorSize());
	}

	D3D12_GPU_DESCRIPTOR_HANDLE D3D12DescriptorAllocation::GetGPUHandle(uint32_t index) const
	{
		GRAPHITE_ASSERT(m_IsValid, "Cannot get handle on an invalid allocation");
		GRAPHITE_ASSERT(!m_CPUOnly, "Cannot get GPU handle on a cpu-only descriptor");
		GRAPHITE_ASSERT(index < m_Count, "Invalid descriptor index");

		const INT offset = static_cast<INT>(m_Index + index);
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_Heap->GetHeap()->GetGPUDescriptorHandleForHeapStart(), offset, m_Heap->GetDescriptorSize());
	}

	ID3D12DescriptorHeap* D3D12DescriptorAllocation::GetHeap() const
	{
		return m_Heap->GetHeap();
	}

	void D3D12DescriptorAllocation::Reset()
	{
		m_Heap = nullptr;
		m_Index = 0;
		m_Count = 0;

		m_CPUOnly = false;

		m_IsValid = false;
	}

	void D3D12DescriptorAllocation::Free()
	{
		if (m_IsValid && m_Heap)
		{
			m_Heap->Free(*this);
		}
	}


	// DescriptorHeap

	D3D12DescriptorHeap::D3D12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t capacity, bool cpuOnly, const wchar_t* name)
	{
		m_Type = type;
		m_Capacity = capacity;
		m_CPUOnly = cpuOnly;

		// RTV and DSV heaps cannot be gpu visible
		if (type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV || type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
			m_CPUOnly = true;

		// TODO: Add assertions for capacity/type combos (sampler heaps have smaller max size)

		// Create the descriptor heap
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = m_Type;
		desc.NumDescriptors = m_Capacity;
		desc.Flags = m_CPUOnly ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 1;

		DX_THROW_IF_FAIL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap)));
		if (name)
		{
			DX_THROW_IF_FAIL(m_Heap->SetName(name));
		}

		m_DescriptorIncrementSize = device->GetDescriptorHandleIncrementSize(m_Type);

		// Currently, the entire heap is free
		m_FreeBlocks.insert({ 0, m_Capacity });
		m_DeferredFrees.resize(GraphicsContext::GetBackBufferCount());
	}

	D3D12DescriptorHeap::~D3D12DescriptorHeap()
	{
		GRAPHITE_ASSERT(m_Count == 0, "Don't destroy allocators that still have memory allocated!!!");

		// Heap will automatically be released
		// GPU must not be using any of the allocations made

		// TODO: defer release of the heap (to make sure GPU is idle when this is released)
	}

	D3D12DescriptorAllocation D3D12DescriptorHeap::Allocate(uint32_t countToAlloc)
	{
		GRAPHITE_ASSERT(countToAlloc > 0, "Invalid quantity of descriptors");

		if (m_Count + countToAlloc > m_Capacity)
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

		m_Count += countToAlloc;

		return D3D12DescriptorAllocation{ this, allocIndex, countToAlloc, m_CPUOnly };
	}

	void D3D12DescriptorHeap::Free(D3D12DescriptorAllocation& allocation)
	{
		if (!allocation.IsValid())
			return;

		// Make sure this heap contains this allocation
		GRAPHITE_ASSERT(allocation.GetHeap() == GetHeap(), "Trying to free an allocation from the wrong heap");

		m_DeferredFrees[m_CurrentDeferredFreeCollection].push_back(std::make_pair(allocation.GetIndex(), allocation.GetCount()));
		allocation.Reset();
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
			m_Count -= freedRange.second;
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