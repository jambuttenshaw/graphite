#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/RHI/RHITypes.h"


namespace Graphite
{
	enum class GPUResourceType : uint8_t
	{
		Invalid = 0,

		// Buffers
		UploadBuffer,
		ByteAddressBuffer,
		StructuredBuffer,
		ReadbackBuffer,

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
	// Handles how the resource should be interpreted and used
	class GPUResource
	{
	protected:
		// GPU resources can only be constructed through the resource manager
		friend class ResourceFactory;
		GPUResource(ResourceAccessFlags accessFlags);
	public:
		GPUResource() = delete;
		virtual ~GPUResource() = default;

		DELETE_COPY(GPUResource);
		DEFAULT_MOVE(GPUResource);

		// Getters
		virtual GPUResourceType GetResourceType() const = 0;
		virtual GPUVirtualAddress GetResourceAddress() const = 0;

		inline ResourceAccessFlags GetAccessFlags() const { return m_AccessFlags; }
		inline bool CheckAccessFlags(ResourceAccessFlags flags) const { return m_AccessFlags & flags; }

	protected:
		// Common properties about this resource, including usage
		ResourceAccessFlags m_AccessFlags = ResourceAccess_None;
	};

}
