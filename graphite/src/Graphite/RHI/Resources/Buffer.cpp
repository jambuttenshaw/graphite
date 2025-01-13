#include "graphite_pch.h"
#include "Buffer.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/GraphicsContext.h"


namespace Graphite
{
	UploadBuffer::UploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride)
	: GPUResource(
		static_cast<ResourceAccessFlags>(ResourceAccess_CPUWrite | ResourceAccess_GPURead)
	)
		, m_ElementCount(elementCount)
		, m_ElementStride(elementStride)
		, m_InstanceCount(instanceCount)
		, m_InstanceStride(elementCount * elementStride)
	{
	}

	uint64_t UploadBuffer::GetElementOffset(uint32_t element, uint32_t instance) const
	{
		GRAPHITE_ASSERT(element < m_ElementCount && instance < m_InstanceCount, "Out of bounds access.");
		return static_cast<uint64_t>(instance * m_InstanceStride) + static_cast<uint64_t>(element * m_ElementStride);
	}


	ByteAddressBuffer::ByteAddressBuffer(bool readOnly)
		: GPUResource(
			static_cast<ResourceAccessFlags>(ResourceAccess_GPURead | (readOnly ? ResourceAccess_None : ResourceAccess_GPUWrite))
		)
	{
		
	}


	ReadbackBuffer::ReadbackBuffer()
		:GPUResource(
			static_cast<ResourceAccessFlags>(ResourceAccess_GPUWrite | ResourceAccess_CPURead)
		)
	{
		
	}


}
