#include "graphite_pch.h"
#include "Buffer.h"


namespace Graphite
{
	UploadBuffer::UploadBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride)
	: GPUResource(
		allocation,
		static_cast<ResourceAccessFlags>(ResourceAccess_CPUWrite | ResourceAccess_GPURead)
	)
		, m_ElementCount(elementCount)
		, m_ElementStride(elementStride)
		, m_InstanceCount(instanceCount)
		, m_InstanceStride(elementCount * elementStride)
	{
		// Map buffer to be able to write to it
		DX_THROW_IF_FAIL(m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
	}

	UploadBuffer::~UploadBuffer()
	{
		if (m_MappedData)
		{
			m_Resource->Unmap(0, nullptr);
		}
	}

	D3D12_GPU_VIRTUAL_ADDRESS UploadBuffer::GetAddressOfElement(uint32_t element, uint32_t instance) const
	{
		return GetAddress() + GetElementOffset(element, instance);
	}

	void UploadBuffer::CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t elementSize) const
	{
		GRAPHITE_ASSERT(elementSize <= m_ElementStride, "Invalid element size!");
		memcpy(m_MappedData + GetElementOffset(element, instance), data, elementSize);
	}

	uint64_t UploadBuffer::GetElementOffset(uint32_t element, uint32_t instance) const
	{
		GRAPHITE_ASSERT(element < m_ElementCount && instance < m_InstanceCount, "Out of bounds access.");
		return static_cast<uint64_t>(instance * m_InstanceStride) + static_cast<uint64_t>(element * m_ElementStride);
	}


	ByteAddressBuffer::ByteAddressBuffer(D3D12MA::Allocation* allocation, bool readOnly)
		: GPUResource(
			allocation,
			static_cast<ResourceAccessFlags>(ResourceAccess_GPURead | (readOnly ? ResourceAccess_None : ResourceAccess_GPUWrite))
		)
	{
		
	}


	ReadbackBuffer::ReadbackBuffer(D3D12MA::Allocation* allocation)
		:GPUResource(
			allocation,
			static_cast<ResourceAccessFlags>(ResourceAccess_GPUWrite | ResourceAccess_CPURead)
		)
	{
		
	}


}
