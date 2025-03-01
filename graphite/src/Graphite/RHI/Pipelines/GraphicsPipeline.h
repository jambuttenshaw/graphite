#pragma once

#include "Graphite/Core/Core.h"

#include "PipelineResourceLayout.h"
#include "PipelineResourceBinding.h"
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

		const std::vector<PipelineResourceDescription>* ResourceLayout;
	};


	class GraphicsPipeline
	{
	protected:
		GRAPHITE_API GraphicsPipeline();
	public:
		GRAPHITE_API virtual ~GraphicsPipeline() = default;

		GRAPHITE_API_DELETE_COPY(GraphicsPipeline);
		GRAPHITE_API_DEFAULT_MOVE(GraphicsPipeline);

		GRAPHITE_API const PipelineResourceSet& GetPipelineResourceSet(PipelineResourceBindingFrequency bindingFrequency) const;

	protected:
		// Non-const used in pipeline creation
		GRAPHITE_API PipelineResourceSet& GetPipelineResourceSet(PipelineResourceBindingFrequency bindingFrequency);

		// Descriptions of the resources used by this pipeline
		// These do not refer to actual resources and their data
		// Instead, they describe how a resource should be bound to a pipeline
		// This data includes which descriptors in a table should be written to
		PipelineResourceSet m_StaticResources;
		PipelineResourceSet m_MutableResources;
		PipelineResourceSet m_DynamicResources;
	};

}
