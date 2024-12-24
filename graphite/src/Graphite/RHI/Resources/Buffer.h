#pragma once
#include "GPUResource.h"

#include "Graphite/Core/Core.h"
#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/RHIExceptions.h"


namespace Graphite
{

	// Upload buffer, templated by the number of elements it contains
	template <typename T>
	class UploadBuffer : public GPUResource
	{
		friend class ResourceFactory;
		UploadBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride)
			: GPUResource(allocation)
			, m_ElementCount(elementCount)
			, m_ElementStride(elementStride)
			, m_InstanceCount(instanceCount)
			, m_InstanceStride(elementCount * elementStride)
		{
			// Map buffer to be able to write to it
			DX_THROW_IF_FAIL(m_Resource->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
		}
	public:
		virtual ~UploadBuffer()
		{
			if (m_MappedData)
			{
				m_Resource->Unmap(0, nullptr);
			}
		};

		DELETE_COPY(UploadBuffer);
		DEFAULT_MOVE(UploadBuffer);

		// Getters
		inline uint32_t GetElementCount() const { return m_ElementCount; }
		inline uint32_t GetElementStride() const { return m_ElementStride; }
		inline uint32_t GetInstanceCount() const { return m_InstanceCount; }
		inline uint32_t GetInstanceStride() const { return m_InstanceStride; }

		inline D3D12_GPU_VIRTUAL_ADDRESS GetAddressOfElement(uint32_t element, uint32_t instance) const
		{
			GRAPHITE_ASSERT(element < m_ElementCount && instance < m_InstanceCount, "Out of bounds access.");
			return GetAddress() + (instance * m_InstanceStride) + (element * m_ElementStride);
		}

		// Populate buffer
		// Note: sizeof(T) likely won't equal element stride, as stride may have been padded
		void CopyElement(uint32_t element, uint32_t instance, const T& data) const
		{
			GRAPHITE_ASSERT(element < m_ElementCount && instance < m_InstanceCount, "Out of bounds access.");
			memcpy(GetMappedAddressOfElement(element, instance), &data, sizeof(T));
		}
		void CopyElements(uint32_t start, uint32_t count, uint32_t instance, const T* const data) const
		{
			GRAPHITE_ASSERT(start + count - 1 < m_ElementCount && instance < m_InstanceCount, "Out of bounds access.");
			memcpy(GetMappedAddressOfElement(start, instance), data, count * sizeof(T));
		}
		void SetElements(uint32_t start, uint32_t count, uint32_t instance, int value) const
		{
			GRAPHITE_ASSERT(start + count - 1 < m_ElementCount && instance < m_InstanceCount, "Out of bounds access.");
			memset(GetMappedAddressOfElement(start, instance), value, count * m_ElementStride);
		}

	private:
		// For writing use only
		// Helper function to find offset
		inline uint8_t* GetMappedAddressOfElement(uint32_t element, uint32_t instance) const
		{
			GRAPHITE_ASSERT(element < m_ElementCount && instance < m_InstanceCount, "Out of bounds access.");
			return m_MappedData + static_cast<uint64_t>(instance * m_InstanceStride) + static_cast<uint64_t>(element * m_ElementStride);
		}

	private:
		uint32_t m_ElementCount;
		uint32_t m_ElementStride;

		uint32_t m_InstanceCount;
		uint32_t m_InstanceStride;

		// Mapped pointer to write to buffer
		uint8_t* m_MappedData;
	};

}
