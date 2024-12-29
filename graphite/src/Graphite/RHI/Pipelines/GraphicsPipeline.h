#pragma once

#include "Graphite/Core/Core.h"

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
	};


	class GraphicsPipeline
	{
	public:
		// Factory
		static std::unique_ptr<GraphicsPipeline> Create(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& pipelineDesc);

	protected:
		GraphicsPipeline() = default;
	public:
		virtual ~GraphicsPipeline() = default;

		DELETE_COPY(GraphicsPipeline);
		DEFAULT_MOVE(GraphicsPipeline);
	};

}
