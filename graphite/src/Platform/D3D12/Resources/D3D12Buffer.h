#pragma once

#include "Graphite/RHI/Resources/Buffer.h"
#include "D3D12Resource.h"


namespace Graphite::D3D12
{
	// Upload buffer, templated by the number of elements it contains
	class D3D12UploadBuffer : public UploadBuffer, public D3D12Resource
	{
	protected:
		friend class D3D12ResourceFactory;
		D3D12UploadBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride);
	public:
		virtual ~D3D12UploadBuffer();

		DELETE_COPY(D3D12UploadBuffer);
		DEFAULT_MOVE(D3D12UploadBuffer);

		// Getters
		inline virtual GPUVirtualAddress GetResourceAddress() const override { return D3D12Resource::GetAddress(); }
		virtual GPUVirtualAddress GetAddressOfElement(uint32_t element, uint32_t instance) const override;

		// Populate buffer
		virtual void CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t dataSize) const override;
		virtual void CopyElements(uint32_t startElement, uint32_t elementCount, uint32_t instance, const void* data, uint64_t dataSize) const override;

	private:
		// Mapped pointer to write to buffer
		uint8_t* m_MappedData;
	};


	class D3D12ConstantBuffer : public ConstantBuffer, public D3D12Resource
	{
	protected:
		friend class D3D12ResourceFactory;
		D3D12ConstantBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t instanceCount, uint32_t elementStride);
	public:
		virtual ~D3D12ConstantBuffer();

		DELETE_COPY(D3D12ConstantBuffer);
		DEFAULT_MOVE(D3D12ConstantBuffer);

		// Getters
		inline virtual GPUVirtualAddress GetResourceAddress() const override { return D3D12Resource::GetAddress(); }
		virtual GPUVirtualAddress GetAddressOfElement(uint32_t element, uint32_t instance) const override;

		// Populate buffer
		virtual void CopyElement(uint32_t element, uint32_t instance, const void* data, uint64_t dataSize) const override;
		virtual void CopyElements(uint32_t startElement, uint32_t elementCount, uint32_t instance, const void* data, uint64_t dataSize) const override;

	private:
		// Mapped pointer to write to buffer
		uint8_t* m_MappedData;
	};


	class D3D12ByteAddressBuffer : public ByteAddressBuffer, public D3D12Resource
	{
	protected:
		friend class D3D12ResourceFactory;
		D3D12ByteAddressBuffer(D3D12MA::Allocation* allocation, bool readOnly)
			: ByteAddressBuffer(readOnly)
			, D3D12Resource(allocation)
		{
		}
	public:
		virtual ~D3D12ByteAddressBuffer() = default;

		DELETE_COPY(D3D12ByteAddressBuffer);
		DEFAULT_MOVE(D3D12ByteAddressBuffer);

		inline virtual GPUVirtualAddress GetResourceAddress() const override { return D3D12Resource::GetAddress(); }
	};


	class D3D12StructuredBuffer : public StructuredBuffer, public D3D12Resource
	{
	protected:
		friend class D3D12ResourceFactory;
		D3D12StructuredBuffer(D3D12MA::Allocation* allocation, uint32_t elementCount, uint32_t elementSize, bool readOnly)
			: StructuredBuffer(elementCount, elementSize, readOnly)
			, D3D12Resource(allocation)
		{
		}
	public:
		virtual ~D3D12StructuredBuffer() = default;

		DELETE_COPY(D3D12StructuredBuffer);
		DEFAULT_MOVE(D3D12StructuredBuffer);

		inline virtual GPUVirtualAddress GetResourceAddress() const override { return D3D12Resource::GetAddress(); }
	};


	class D3D12ReadbackBuffer : public ReadbackBuffer, public D3D12Resource
	{
	protected:
		friend class D3D12ResourceFactory;
		D3D12ReadbackBuffer(D3D12MA::Allocation* allocation)
			: D3D12Resource(allocation)
		{
		}
	public:
		virtual ~D3D12ReadbackBuffer() = default;

		DELETE_COPY(D3D12ReadbackBuffer);
		DEFAULT_MOVE(D3D12ReadbackBuffer);

		inline virtual GPUVirtualAddress GetResourceAddress() const override { return D3D12Resource::GetAddress(); }
	};
}
