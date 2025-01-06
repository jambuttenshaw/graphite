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

		//inline const ResourceViewList* GetBoundStaticResources() const { return m_BoundStaticResources; }
		//inline void BindStaticResources(ResourceViewList* resources) { m_BoundStaticResources = resources; }

	protected:
		// Description of resources used by this pipeline
		//std::vector<ResourceTable> m_StaticResources;

		// The static resources to be bound to the pipeline
		// Mutable and dynamic resources are handled through resource binders rather than the pipeline themselves,
		// to minimize the amount of swapping that will occur
		//ResourceViewList* m_BoundStaticResources = nullptr;
	};

}
