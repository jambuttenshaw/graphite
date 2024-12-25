#include "pch.h"
#include "GPUResource.h"

#include "D3D12MemAlloc.h"


namespace Graphite
{

	GPUResource::GPUResource(D3D12MA::Allocation* allocation, ResourceAccessFlags accessFlags)
		: m_Allocation(allocation)
		, m_Resource(allocation->GetResource())
		, m_AccessFlags(accessFlags)
	{
		
	}

	GPUResource::~GPUResource()
	{
		m_Allocation->Release();
	}



}
