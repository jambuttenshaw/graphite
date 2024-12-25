#pragma once

#include "Graphite/Core/Core.h"


namespace D3D12MA
{
	class Allocation;
}


namespace Graphite
{
	enum class GPUResourceType : uint8_t
	{
		Invalid = 0,

		// Buffers
		UploadBuffer,
		ConstantBuffer,
		ByteAddressBuffer,
		StructuredBuffer,
		ReadbackBuffer,

		VertexBuffer,
		IndexBuffer,

		// Textures
		Texture1D,
		Texture2D,
		Texture3D
	};

	// Flags used to determine how a resource can be accessed
	enum ResourceAccessFlags : uint8_t
	{
		ResourceAccess_None		= 0x0,
		ResourceAccess_CPURead	= 0x1,
		ResourceAccess_CPUWrite	= 0x2,
		ResourceAccess_GPURead	= 0x4,
		ResourceAccess_GPUWrite	= 0x8,
	};


	// A base class representing any resource that can be used by the GPU
	class GPUResource
	{
	protected:
		// GPU resources can only be constructed through the resource manager
		friend class ResourceFactory;
		GPUResource(D3D12MA::Allocation* allocation, ResourceAccessFlags accessFlags);
	public:
		GPUResource() = delete;
		virtual ~GPUResource();

		DELETE_COPY(GPUResource);
		DEFAULT_MOVE(GPUResource);

		// Getters
		virtual GPUResourceType GetResourceType() const = 0;

		inline ResourceAccessFlags GetAccessFlags() const { return m_AccessFlags; }
		inline bool CheckAccessFlags(ResourceAccessFlags flags) const { return m_AccessFlags & flags; }

		inline D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const { return m_Resource->GetGPUVirtualAddress(); }
		inline ID3D12Resource* GetResource() const { return m_Resource; }

	protected:
		// The underlying allocation supporting this resource
		D3D12MA::Allocation* m_Allocation = nullptr;
		// The actual D3D12 resource
		ID3D12Resource* m_Resource = nullptr;

		// Common properties about this resource, including usage
		ResourceAccessFlags m_AccessFlags = ResourceAccess_None;
	};

}
