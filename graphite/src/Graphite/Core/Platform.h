#pragma once

#include "Core.h"

#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/Resources/ResourceFactory.h"
#include "Graphite/RHI/Pipelines/ShaderCompiler.h"
#include "Graphite/ImGui/ImGuiBackend.h"


namespace Graphite
{

	class Platform
	{
	public:
		GRAPHITE_API Platform() = default;
		GRAPHITE_API virtual ~Platform() = default;

		GRAPHITE_API_DELETE_COPY(Platform);
		GRAPHITE_API_DELETE_MOVE(Platform);

		// Called by application to set up platform
		GRAPHITE_API virtual void InitPlatform(
			const struct GraphicsContextDesc& graphicsContextDesc
		) {};

		// Getters
		GRAPHITE_API GraphicsContext* GetGraphicsContext() const { return m_GraphicsContext.get(); }
		GRAPHITE_API ResourceFactory* GetResourceFactory() const { return m_ResourceFactory.get(); }
		GRAPHITE_API ShaderCompiler* GetShaderCompiler() const { return m_ShaderCompiler.get(); }
		GRAPHITE_API ImGuiBackend* GetImGuiBackend() const { return m_ImGuiBackend.get(); }

	protected:
		// All platform-specific objects that need accessed in the core engine live here
		std::unique_ptr<GraphicsContext> m_GraphicsContext;
		std::unique_ptr<ResourceFactory> m_ResourceFactory;
		std::unique_ptr<ShaderCompiler> m_ShaderCompiler;
		std::unique_ptr<ImGuiBackend> m_ImGuiBackend;
	};

}
