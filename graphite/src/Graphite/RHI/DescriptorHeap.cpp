#include "graphite_pch.h"
#include "DescriptorHeap.h"

#include "Graphite/Core/Assert.h"


namespace Graphite
{

	DescriptorAllocation::DescriptorAllocation(DescriptorHeap* heap, uint32_t index, uint32_t count, bool cpuOnly)
		: m_Heap(heap)
		, m_Index(index)
		, m_Count(count)
		, m_CPUOnly(cpuOnly)
		, m_IsValid(true)
	{
	}

	DescriptorAllocation::~DescriptorAllocation()
	{
		if (IsValid())
		{
			Free();
		}
	}


	DescriptorAllocation::DescriptorAllocation(DescriptorAllocation&& other) noexcept
	{
		m_Heap = std::move(other.m_Heap);
		m_Index = std::move(other.m_Index);
		m_Count = std::move(other.m_Count);

		m_CPUOnly = std::move(other.m_CPUOnly);

		m_IsValid = std::move(other.m_IsValid);

		other.ResetWithoutFree();
	}

	DescriptorAllocation& DescriptorAllocation::operator=(DescriptorAllocation&& other) noexcept
	{
		m_Heap = std::move(other.m_Heap);
		m_Index = std::move(other.m_Index);
		m_Count = std::move(other.m_Count);

		m_CPUOnly = std::move(other.m_CPUOnly);

		m_IsValid = std::move(other.m_IsValid);

		other.ResetWithoutFree();

		return *this;
	}


	GPUDescriptorHandle DescriptorAllocation::GetCPUHandle(uint32_t index) const
	{
		GRAPHITE_ASSERT(m_IsValid, "Cannot get handle on an invalid allocation");
		GRAPHITE_ASSERT(index < m_Count, "Invalid descriptor index");

		const INT offset = static_cast<INT>(m_Index + index);
		return m_Heap->GetCPUHandleForHeapStart() + (offset * m_Heap->GetDescriptorSize());
	}

	GPUDescriptorHandle DescriptorAllocation::GetGPUHandle(uint32_t index) const
	{
		GRAPHITE_ASSERT(m_IsValid, "Cannot get handle on an invalid allocation");
		GRAPHITE_ASSERT(!m_CPUOnly, "Cannot get GPU handle on a cpu-only descriptor");
		GRAPHITE_ASSERT(index < m_Count, "Invalid descriptor index");

		const INT offset = static_cast<INT>(m_Index + index);
		return m_Heap->GetGPUHandleForHeapStart() + (offset * m_Heap->GetDescriptorSize());
	}

	DescriptorHeap* DescriptorAllocation::GetHeap() const
	{
		return m_Heap;
	}

	void DescriptorAllocation::ResetWithoutFree()
	{
		m_Heap = nullptr;
		m_Index = 0;
		m_Count = 0;

		m_CPUOnly = false;

		m_IsValid = false;
	}

	void DescriptorAllocation::Free()
	{
		if (m_IsValid && m_Heap)
		{
			m_Heap->Free(*this);
		}
	}


	DescriptorHeap::DescriptorHeap(DescriptorHeapType heapType, uint32_t descriptorSize, uint32_t capacity, bool cpuOnly)
		: m_HeapType(heapType)
		, m_DescriptorSize(descriptorSize)
		, m_Capacity(capacity)
		, m_CountAllocated(0)
		, m_CPUOnly(cpuOnly)
	{
	}

}
