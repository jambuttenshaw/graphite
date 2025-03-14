#include "graphite_d3d12_pch.h"
#include "D3D12Texture.h"


namespace Graphite::D3D12
{

	D3D12Texture2D::D3D12Texture2D(D3D12MA::Allocation* allocation, const Texture2DDesc& desc)
		: Texture2D(desc)
		, D3D12Resource(allocation)
	{
		
	}


}