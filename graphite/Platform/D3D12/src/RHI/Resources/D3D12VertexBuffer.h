#pragma once

#include "Graphite/RHI/Resources/VertexBuffer.h"
#include "D3D12Resource.h"


namespace Graphite::D3D12
{

	class D3D12VertexBuffer : public VertexBuffer, D3D12Resource
	{
	protected:
		friend class D3D12ResourceFactory;
		D3D12VertexBuffer(D3D12MA::Allocation* allocation, uint32_t vertexCount, const InputLayout& inputLayout);
	public:
		virtual ~D3D12VertexBuffer();

		DELETE_COPY(D3D12VertexBuffer);
		DEFAULT_MOVE(D3D12VertexBuffer);

		// Getters
		inline virtual GPUVirtualAddress GetResourceAddress() const override { return D3D12Resource::GetAddress(); }

		inline std::span<const D3D12_VERTEX_BUFFER_VIEW> GetNativeViews() const { return m_NativeViews; }

	private:
		// Mapped pointer to write to buffer
		uint8_t* m_MappedData;

		// Native vertex buffer views
		std::vector<D3D12_VERTEX_BUFFER_VIEW> m_NativeViews;
	};

}
