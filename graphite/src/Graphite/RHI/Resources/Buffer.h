#pragma once

#include "GPUResource.h"

#include "Graphite/Core/Core.h"


namespace Graphite
{

	// Upload buffer, templated by the number of elements it contains
	class UploadBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		GRAPHITE_API UploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride);
	public:
		GRAPHITE_API virtual ~UploadBuffer() = default;

		GRAPHITE_API_DELETE_COPY(UploadBuffer);
		GRAPHITE_API_DEFAULT_MOVE(UploadBuffer);

		// Getters
		GRAPHITE_API inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::UploadBuffer; }

		GRAPHITE_API inline uint32_t GetElementCount() const { return m_ElementCount; }
		GRAPHITE_API inline uint32_t GetElementStride() const { return m_ElementStride; }

		GRAPHITE_API inline uint32_t GetInstanceCount() const { return m_InstanceCount; }
		GRAPHITE_API inline uint32_t GetInstanceStride() const { return m_InstanceStride; }

		GRAPHITE_API virtual GPUVirtualAddress GetAddressOfElement(uint32_t element, uint32_t instance) const = 0;

		// Populate buffer
		GRAPHITE_API virtual void CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t dataSize) const = 0;
		GRAPHITE_API virtual void CopyElements(uint32_t startElement, uint32_t elementCount, uint32_t instance, const void* data, uint64_t dataSize) const = 0;

	protected:
		// For writing use only
		// Helper function to find offset
		GRAPHITE_API uint64_t GetElementOffset(uint32_t element, uint32_t instance) const;

	protected:
		uint32_t m_ElementCount;
		uint32_t m_ElementStride;

		uint32_t m_InstanceCount;
		uint32_t m_InstanceStride;
	};


	class ByteAddressBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		GRAPHITE_API ByteAddressBuffer(bool readOnly);
	public:
		GRAPHITE_API virtual ~ByteAddressBuffer() = default;

		GRAPHITE_API_DELETE_COPY(ByteAddressBuffer);
		GRAPHITE_API_DEFAULT_MOVE(ByteAddressBuffer);

		GRAPHITE_API inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::ByteAddressBuffer; }
	};


	class StructuredBuffer : public ByteAddressBuffer
	{
	protected:
		friend class ResourceFactory;
		GRAPHITE_API StructuredBuffer(uint32_t elementCount, uint32_t elementSize, bool readOnly)
			: ByteAddressBuffer(readOnly)
			, m_ElementCount(elementCount)
			, m_ElementSize(elementSize)
		{}
	public:
		GRAPHITE_API virtual ~StructuredBuffer() = default;

		GRAPHITE_API_DELETE_COPY(StructuredBuffer);
		GRAPHITE_API_DEFAULT_MOVE(StructuredBuffer);

		GRAPHITE_API inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::StructuredBuffer; }

		GRAPHITE_API inline uint32_t GetElementCount() const { return m_ElementCount; }
		GRAPHITE_API inline uint32_t GetElementSize() const { return m_ElementSize; }

	private:
		uint32_t m_ElementCount;
		uint32_t m_ElementSize;
	};


	class ReadbackBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		GRAPHITE_API ReadbackBuffer();
	public:
		GRAPHITE_API virtual ~ReadbackBuffer() = default;

		GRAPHITE_API_DELETE_COPY(ReadbackBuffer);
		GRAPHITE_API_DEFAULT_MOVE(ReadbackBuffer);

		GRAPHITE_API inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::ReadbackBuffer; }
	};

}
