#pragma once

#include "Graphite/RHI/Resources/ResourceFactory.h"


namespace D3D12MA
{
	class Allocator;
	class Allocation;
}

namespace Graphite
{
	class GraphicsContext;
}

namespace Graphite::D3D12
{
	extern ResourceFactory* CreateD3D12ResourceFactory(const GraphicsContext&);

	class D3D12ResourceFactory : public ResourceFactory
	{
	public:
		D3D12ResourceFactory(const GraphicsContext& graphicsContext);
		virtual ~D3D12ResourceFactory();

		DELETE_COPY(D3D12ResourceFactory);
		DELETE_MOVE(D3D12ResourceFactory);

		// Buffers
		virtual std::unique_ptr<UploadBuffer> CreateUploadBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize, uint32_t elementAlignment) const override;
		virtual std::unique_ptr<ConstantBuffer> CreateConstantBuffer(uint32_t elementCount, uint32_t instanceCount, uint32_t elementSize) const override;
		virtual std::unique_ptr<ByteAddressBuffer> CreateByteAddressBuffer(uint64_t width, bool readOnly) const override;
		virtual std::unique_ptr<StructuredBuffer> CreateStructuredBuffer(uint32_t elementCount, uint32_t elementSize, bool readOnly) const override;
		virtual std::unique_ptr<ReadbackBuffer> CreateReadbackBuffer(uint64_t width) const override;

	private:
		D3D12MA::Allocation* AllocateBuffer(D3D12_HEAP_TYPE heap, uint64_t width, D3D12_RESOURCE_FLAGS flags) const;

	private:
		D3D12MA::Allocator* m_Allocator = nullptr;
	};
}
