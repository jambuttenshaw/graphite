#pragma once

#include "Graphite/Core/Core.h"

#include "PipelineResourceLayout.h"
#include "ShaderCompiler.h"


namespace Graphite
{
	class GraphicsContext;
	class InputLayout;

	struct GraphicsPipelineDescription
	{
		const InputLayout* InputVertexLayout;

		ShaderDescription VertexShader;
		ShaderDescription PixelShader;

		const PipelineResourceLayout* ResourceLayout;
	};


	class GraphicsPipeline
	{
	public:
		// Factory
		GRAPHITE_API static std::unique_ptr<GraphicsPipeline> Create(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& pipelineDesc);

	protected:
		GraphicsPipeline() = default;
	public:
		virtual ~GraphicsPipeline() = default;

		DELETE_COPY(GraphicsPipeline);
		DEFAULT_MOVE(GraphicsPipeline);

		inline const ResourceViewList* GetStaticResources() const { return m_StaticResources; }
		inline void SetStaticResources(ResourceViewList* resources) { m_StaticResources = resources; }

	protected:
		ResourceViewList* m_StaticResources = nullptr;
	};

}
