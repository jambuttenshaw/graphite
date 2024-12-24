#pragma once

#include "Graphite/Core/Core.h"


namespace D3D12MA
{
	class Allocation;
}


namespace Graphite
{
	// A base class representing any resource that can be used by the GPU
	class GPUResource
	{
	protected:
		// GPU resources can only be constructed through the resource manager
		friend class ResourceFactory;
		GPUResource(D3D12MA::Allocation* allocation);
	public:
		GPUResource() = delete;
		virtual ~GPUResource();

		DELETE_COPY(GPUResource);
		DEFAULT_MOVE(GPUResource);

		// Getters
		inline D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const { return m_Resource->GetGPUVirtualAddress(); }
		inline ID3D12Resource* GetResource() const { return m_Resource; }

	protected:
		// The underlying allocation supporting this resource
		D3D12MA::Allocation* m_Allocation = nullptr;
		// The actual D3D12 resource
		ID3D12Resource* m_Resource = nullptr;

		// Common properties about this resource, including usage
	};

}
