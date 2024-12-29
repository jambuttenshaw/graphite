#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/RHI/DescriptorHeap.h"

using Microsoft::WRL::ComPtr;


namespace Graphite::D3D12
{

	class D3D12DescriptorHeap;


	class D3D12DescriptorHeap : public DescriptorHeap
	{
	public:
		D3D12DescriptorHeap(ID3D12Device* device, DescriptorHeapType type, uint32_t capacity, bool cpuOnly, const wchar_t* name);
		virtual ~D3D12DescriptorHeap();

		// cannot be copied or moved
		DELETE_COPY(D3D12DescriptorHeap);
		DEFAULT_MOVE(D3D12DescriptorHeap);

		virtual DescriptorAllocation Allocate(uint32_t countToAlloc) override;
		virtual void Free(DescriptorAllocation& allocation) override;

		virtual CPUDescriptorHandle GetCPUHandleForHeapStart() const override;
		virtual GPUDescriptorHandle GetGPUHandleForHeapStart() const override;

		void ProcessDeferredFrees(uint32_t frameIndex);

		// Getters
		inline ID3D12DescriptorHeap* GetNativeHeap() const { return m_NativeHeap.Get(); }
		inline D3D12_DESCRIPTOR_HEAP_TYPE GetNativeHeapType() const { return m_NativeHeapType; }

	private:
		// In this design, a D3DDescriptor heap owns its entire heap
		ComPtr<ID3D12DescriptorHeap> m_NativeHeap;		// The heap to allocate from
		D3D12_DESCRIPTOR_HEAP_TYPE m_NativeHeapType;	// The type of descriptors heap contains

		std::map<uint32_t, uint32_t> m_FreeBlocks; // Map of index to size of free blocks

		using DeferredFreeCollection = std::vector<std::pair<uint32_t, uint32_t>>;
		std::vector<DeferredFreeCollection> m_DeferredFrees; // One collection is required for each frame in flight
		uint32_t m_CurrentDeferredFreeCollection = 0;
	};

}