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
		UploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride);
	public:
		virtual ~UploadBuffer() = default;

		DELETE_COPY(UploadBuffer);
		DEFAULT_MOVE(UploadBuffer);

		// Getters
		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::UploadBuffer; }

		inline uint32_t GetElementCount() const { return m_ElementCount; }
		inline uint32_t GetElementStride() const { return m_ElementStride; }

		inline uint32_t GetInstanceCount() const { return m_InstanceCount; }
		inline uint32_t GetInstanceStride() const { return m_InstanceStride; }

		virtual GraphiteGPUVirtualAddress GetAddressOfElement(uint32_t element, uint32_t instance) const = 0;

		// Populate buffer
		virtual void CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t dataSize) const = 0;
		virtual void CopyElements(uint32_t startElement, uint32_t elementCount, uint32_t instance, const void* data, uint64_t dataSize) const = 0;

	protected:
		// For writing use only
		// Helper function to find offset
		uint64_t GetElementOffset(uint32_t element, uint32_t instance) const;

	protected:
		uint32_t m_ElementCount;
		uint32_t m_ElementStride;

		uint32_t m_InstanceCount;
		uint32_t m_InstanceStride;
	};


	class ConstantBuffer : public UploadBuffer
	{
	protected:
		friend class ResourceFactory;
		ConstantBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride)
			: UploadBuffer(elementCount, instanceCount, elementStride)
		{}
	public:
		virtual ~ConstantBuffer() = default;

		DELETE_COPY(ConstantBuffer);
		DEFAULT_MOVE(ConstantBuffer);

		// Getters
		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::ConstantBuffer; }
	};


	class ByteAddressBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		ByteAddressBuffer(bool readOnly);
	public:
		virtual ~ByteAddressBuffer() = default;

		DELETE_COPY(ByteAddressBuffer);
		DEFAULT_MOVE(ByteAddressBuffer);

		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::ByteAddressBuffer; }
	};


	class StructuredBuffer : public ByteAddressBuffer
	{
	protected:
		friend class ResourceFactory;
		StructuredBuffer(uint32_t elementCount, bool readOnly)
			: ByteAddressBuffer(readOnly)
			, m_ElementCount(elementCount)
		{}
	public:
		virtual ~StructuredBuffer() = default;

		DELETE_COPY(StructuredBuffer);
		DEFAULT_MOVE(StructuredBuffer);

		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::StructuredBuffer; }
		inline uint32_t GetElementCount() const { return m_ElementCount; }

	private:
		uint32_t m_ElementCount;
	};


	class ReadbackBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		ReadbackBuffer();
	public:
		virtual ~ReadbackBuffer() = default;

		DELETE_COPY(ReadbackBuffer);
		DEFAULT_MOVE(ReadbackBuffer);

		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::ReadbackBuffer; }
	};

}
