#pragma once
#include "Graphite/Core/Core.h"


namespace D3D12MA
{
	class Allocation;
}


namespace Graphite::D3D12
{

	// The base class of any object that owns memory allocated through the D3D12 memory allocator
	class D3D12Resource
	{
	protected:
		D3D12Resource(D3D12MA::Allocation* allocation);
	public:
		D3D12Resource() = delete;
		virtual ~D3D12Resource();

		DELETE_COPY(D3D12Resource);
		DEFAULT_MOVE(D3D12Resource);

		// D3D12 API
		inline ID3D12Resource* GetResource() const { return m_Resource; }
		inline D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const { return m_Resource->GetGPUVirtualAddress(); }

	protected:
		// The underlying allocation supporting this resource
		D3D12MA::Allocation* m_Allocation = nullptr;
		// Native handle to the resource
		ID3D12Resource* m_Resource;
	};
}
