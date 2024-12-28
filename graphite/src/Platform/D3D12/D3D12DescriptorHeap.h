#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/RHI/DescriptorHeap.h"

using Microsoft::WRL::ComPtr;


namespace Graphite::D3D12
{

	class D3D12DescriptorHeap;


	class D3D12DescriptorAllocation
	{
	public:
		D3D12DescriptorAllocation() = default;
		D3D12DescriptorAllocation(
			D3D12DescriptorHeap* heap,
			uint32_t index,
			uint32_t count,
			bool cpuOnly
		);
		~D3D12DescriptorAllocation();

		DELETE_COPY(D3D12DescriptorAllocation)
		D3D12DescriptorAllocation(D3D12DescriptorAllocation&& other) noexcept;
		D3D12DescriptorAllocation& operator=(D3D12DescriptorAllocation&& other) noexcept;

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index = 0) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index = 0) const;

		void Reset();
		void Free();

		inline bool IsValid() const { return m_IsValid; }

		ID3D12DescriptorHeap* GetHeap() const;
		inline uint32_t GetIndex() const { return m_Index; }
		inline uint32_t GetCount() const { return m_Count; }

	private:
		D3D12DescriptorHeap* m_Heap = nullptr;	// The allocator that made this allocation
		uint32_t m_Index = 0;						// The index this allocation begins at
		uint32_t m_Count = 0;						// The number of descriptors in this allocation

		bool m_CPUOnly = false;					// Is this heap CPU only

		bool m_IsValid = false;					// Does this allocation actually point to a real allocated descriptor
	};


	class D3D12DescriptorHeap : public DescriptorHeap
	{
	public:
		D3D12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t capacity, bool cpuOnly, const wchar_t* name);
		virtual ~D3D12DescriptorHeap();

		// cannot be copied or moved
		DELETE_COPY(D3D12DescriptorHeap);
		DEFAULT_MOVE(D3D12DescriptorHeap);

		D3D12DescriptorAllocation Allocate(uint32_t countToAlloc);
		void Free(D3D12DescriptorAllocation& allocation);

		void ProcessDeferredFrees(uint32_t frameIndex);

		// Getters
		inline ID3D12DescriptorHeap* GetHeap() const { return m_Heap.Get(); }
		inline D3D12_DESCRIPTOR_HEAP_TYPE GetType() const { return m_Type; }
		inline uint32_t GetDescriptorSize() const { return m_DescriptorIncrementSize; }

		inline uint32_t GetCapacity() const { return m_Capacity; }
		inline uint32_t GetCount() const { return m_Count; }

	private:
		// In this design, a D3DDescriptor heap owns its entire heap
		ComPtr<ID3D12DescriptorHeap> m_Heap;	// The heap to allocate from

		D3D12_DESCRIPTOR_HEAP_TYPE m_Type;		// The type of descriptors heap contains
		uint32_t m_DescriptorIncrementSize = 0;

		uint32_t m_Capacity = 0;
		uint32_t m_Count = 0;

		bool m_CPUOnly = false;

		std::map<uint32_t, uint32_t> m_FreeBlocks; // Map of index to size of free blocks

		using DeferredFreeCollection = std::vector<std::pair<uint32_t, uint32_t>>;
		std::vector<DeferredFreeCollection> m_DeferredFrees; // One collection is required for each frame in flight
		uint32_t m_CurrentDeferredFreeCollection = 0;
	};

}