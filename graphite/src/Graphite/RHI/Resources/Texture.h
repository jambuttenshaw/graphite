#pragma once

#include "Graphite/Core/Core.h"
#include "GPUResource.h"


namespace Graphite
{

	struct TextureClearValue
	{
		// TODO: Doesn't need to contain color AND stencil
		glm::vec4 Color;
		float Depth;
		uint8_t Stencil;
	};


	struct Texture2DDesc
	{
		uint32_t Width;
		uint32_t Height;

		GraphiteFormat Format;
		std::optional<TextureClearValue> ClearValue;

		ResourceAccessFlags AccessFlags;
	};

	class Texture2D : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		GRAPHITE_API Texture2D(const Texture2DDesc& desc);
	public:
		GRAPHITE_API virtual ~Texture2D() = default;

		GRAPHITE_API_DELETE_COPY(Texture2D);
		GRAPHITE_API_DEFAULT_MOVE(Texture2D);

		GPUResourceType GetResourceType() const override { return GPUResourceType::Texture2D; }

	protected:
		uint32_t m_Width, m_Height;
	};

}