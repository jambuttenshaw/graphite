#pragma once

#include "GPUResource.h"

#include "Graphite/Core/Core.h"
#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/RHIExceptions.h"


namespace Graphite
{

	// Upload buffer, templated by the number of elements it contains
	class UploadBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		UploadBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride);
	public:
		virtual ~UploadBuffer();

		DELETE_COPY(UploadBuffer);
		DEFAULT_MOVE(UploadBuffer);

		// Getters
		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::UploadBuffer; }

		inline uint32_t GetElementCount() const { return m_ElementCount; }
		inline uint32_t GetElementStride() const { return m_ElementStride; }

		inline uint32_t GetInstanceCount() const { return m_InstanceCount; }
		inline uint32_t GetInstanceStride() const { return m_InstanceStride; }

		D3D12_GPU_VIRTUAL_ADDRESS GetAddressOfElement(uint32_t element, uint32_t instance) const;

		// Populate buffer
		void CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t elementSize) const;

	private:
		// For writing use only
		// Helper function to find offset
		uint64_t GetElementOffset(uint32_t element, uint32_t instance) const;

	private:
		uint32_t m_ElementCount;
		uint32_t m_ElementStride;

		uint32_t m_InstanceCount;
		uint32_t m_InstanceStride;

		// Mapped pointer to write to buffer
		uint8_t* m_MappedData;
	};


	class ConstantBuffer : public UploadBuffer
	{
	protected:
		friend class ResourceFactory;
		ConstantBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride)
			: UploadBuffer(allocation, elementCount, instanceCount, elementStride)
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
		ByteAddressBuffer(D3D12MA::Allocation* allocation, bool readOnly);
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
		StructuredBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, bool readOnly)
			: ByteAddressBuffer(allocation, readOnly)
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
		ReadbackBuffer(D3D12MA::Allocation* allocation);
	public:
		virtual ~ReadbackBuffer() = default;

		DELETE_COPY(ReadbackBuffer);
		DEFAULT_MOVE(ReadbackBuffer);

		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::ReadbackBuffer; }
	};

}
