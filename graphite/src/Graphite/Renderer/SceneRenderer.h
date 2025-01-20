#pragma once

#include "RendererInterface.h"


namespace Graphite
{

	class SceneRenderer : public RendererInterface
	{
	public:
		GRAPHITE_API SceneRenderer() = default;
		GRAPHITE_API virtual ~SceneRenderer() = default;

		GRAPHITE_API_DELETE_COPY(SceneRenderer);
		GRAPHITE_API_DEFAULT_MOVE(SceneRenderer);

		// RendererInterface implementation
		GRAPHITE_API virtual void PreRender() override {}
		GRAPHITE_API virtual void Render() override;
		GRAPHITE_API virtual void PostRender() override {}
	};

}
