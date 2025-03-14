#pragma once

#include "Graphite/RHI/Resources/Texture.h"
#include "D3D12Resource.h"


namespace Graphite::D3D12
{

	class D3D12Texture2D : public Texture2D, public D3D12Resource
	{
		friend class D3D12ResourceFactory;
		D3D12Texture2D(D3D12MA::Allocation* allocation, const Texture2DDesc& desc);
	public:
		virtual ~D3D12Texture2D() = default;

		DELETE_COPY(D3D12Texture2D)
		DEFAULT_MOVE(D3D12Texture2D)

		inline virtual GPUVirtualAddress GetResourceAddress() const override { return D3D12Resource::GetAddress(); }

	private:

	};

}
