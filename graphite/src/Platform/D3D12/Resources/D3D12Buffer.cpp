#include "graphite_pch.h"
#include "D3D12Buffer.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/RHIExceptions.h"


namespace Graphite::D3D12
{

	D3D12UploadBuffer::D3D12UploadBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride)
		: UploadBuffer(elementCount, instanceCount, elementStride)
		, D3D12Resource(allocation)
	{
		DX_THROW_IF_FAIL(m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
	}

	D3D12UploadBuffer::~D3D12UploadBuffer()
	{
		if (m_MappedData)
		{
			m_Resource->Unmap(0, nullptr);
		}
	}

	GraphiteGPUVirtualAddress D3D12UploadBuffer::GetAddressOfElement(uint32_t element, uint32_t instance) const
	{
		return GetResourceAddress() + GetElementOffset(element, instance);
	}

	void D3D12UploadBuffer::CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t dataSize) const
	{
		memcpy(m_MappedData + GetElementOffset(element, instance), data, dataSize);
	}

	void D3D12UploadBuffer::CopyElements(uint32_t startElement, uint32_t elementCount, uint32_t instance, const void* data, uint64_t dataSize) const
	{
		GRAPHITE_ASSERT(startElement + elementCount <= m_ElementCount, "Out of bounds access.");
		GRAPHITE_ASSERT(instance <= m_InstanceCount, "Out of bounds access.");
		memcpy(m_MappedData + GetElementOffset(startElement, instance), data, dataSize);
	}


	D3D12ConstantBuffer::D3D12ConstantBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride)
		: ConstantBuffer(elementCount, instanceCount, elementStride)
		, D3D12Resource(allocation)
	{
		DX_THROW_IF_FAIL(m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
	}

	D3D12ConstantBuffer::~D3D12ConstantBuffer()
	{
		if (m_MappedData)
		{
			m_Resource->Unmap(0, nullptr);
		}
	}

	GraphiteGPUVirtualAddress D3D12ConstantBuffer::GetAddressOfElement(uint32_t element, uint32_t instance) const
	{
		return GetResourceAddress() + GetElementOffset(element, instance);
	}

	void D3D12ConstantBuffer::CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t dataSize) const
	{
		memcpy(m_MappedData + GetElementOffset(element, instance), data, dataSize);
	}

	void D3D12ConstantBuffer::CopyElements(uint32_t startElement, uint32_t elementCount, uint32_t instance, const void* data, uint64_t dataSize) const
	{
		GRAPHITE_ASSERT(startElement + elementCount <= m_ElementCount, "Out of bounds access.");
		GRAPHITE_ASSERT(instance <= m_InstanceCount, "Out of bounds access.");
		memcpy(m_MappedData + GetElementOffset(startElement, instance), data, dataSize);
	}

}
