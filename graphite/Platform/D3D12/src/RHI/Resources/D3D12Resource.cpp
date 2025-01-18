#include "graphite_d3d12_pch.h"
#include "D3D12Resource.h"

#include "D3D12MemAlloc.h"


namespace Graphite::D3D12
{
	D3D12Resource::D3D12Resource(D3D12MA::Allocation* allocation)
		: m_Allocation(allocation)
		, m_Resource(allocation->GetResource())
	{
	}

	D3D12Resource::~D3D12Resource()
	{
		m_Allocation->Release();
	}


}
