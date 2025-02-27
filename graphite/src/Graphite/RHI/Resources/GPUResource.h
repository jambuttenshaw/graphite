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

		// Vertex buffer is handled differently
		VertexBuffer,

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
		GRAPHITE_API GPUResource(ResourceAccessFlags accessFlags);
	public:
		GRAPHITE_API GPUResource() = delete;
		GRAPHITE_API virtual ~GPUResource() = default;

		GRAPHITE_API_DELETE_COPY(GPUResource);
		GRAPHITE_API_DEFAULT_MOVE(GPUResource);

		// Getters
		GRAPHITE_API virtual GPUResourceType GetResourceType() const = 0;
		GRAPHITE_API virtual GPUVirtualAddress GetResourceAddress() const = 0;

		GRAPHITE_API inline ResourceAccessFlags GetAccessFlags() const { return m_AccessFlags; }
		GRAPHITE_API inline bool CheckAccessFlags(ResourceAccessFlags flags) const { return m_AccessFlags & flags; }

	protected:
		// Common properties about this resource, including usage
		ResourceAccessFlags m_AccessFlags = ResourceAccess_None;
	};

}
