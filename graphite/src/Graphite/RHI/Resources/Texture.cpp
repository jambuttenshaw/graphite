#include "graphite_pch.h"
#include "Texture.h"

#include "Core/Assert.h"


namespace Graphite
{

	Texture2D::Texture2D(const Texture2DDesc& desc)
		: GPUResource(desc.AccessFlags)
		, m_Width(desc.Width)
		, m_Height(desc.Height)
	{
		GRAPHITE_ASSERT(!CheckAccessFlags(ResourceAccess_CPURead), "CPU Read is not supported for textures.");
		GRAPHITE_ASSERT(!CheckAccessFlags(ResourceAccess_CPUWrite), "CPU Write is not supported for textures.");
	}


}
