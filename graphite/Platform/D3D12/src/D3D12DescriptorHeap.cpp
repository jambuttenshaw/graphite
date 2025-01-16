#include "graphite_d3d12_pch.h"
#include "D3D12DescriptorHeap.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/GraphicsContext.h"

#include "D3D12Types.h"
#include "D3D12Exceptions.h"


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
	}

	D3D12DescriptorHeap::~D3D12DescriptorHeap()
	{
		// Heap will automatically be released
		// GPU must not be using any of the allocations made

		// TODO: defer release of the heap (to make sure GPU is idle when this is released)
		// TODO: Ensure all allocators for this heap have freed all of their allocations
	}

	CPUDescriptorHandle D3D12DescriptorHeap::GetCPUHandleForHeapStart() const
	{
		return D3D12CPUDescriptorToGraphiteDescriptor(m_NativeHeap->GetCPUDescriptorHandleForHeapStart());
	}

	GPUDescriptorHandle D3D12DescriptorHeap::GetGPUHandleForHeapStart() const
	{
		return D3D12GPUDescriptorToGraphiteDescriptor(m_NativeHeap->GetGPUDescriptorHandleForHeapStart());
	}
}