#include "graphite_pch.h"
#include "GraphicsPipeline.h"

#include "Platform/D3D12/Pipelines/D3D12GraphicsPipeline.h"


namespace Graphite
{

	std::unique_ptr<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& pipelineDesc)
	{
		return std::unique_ptr<D3D12::D3D12GraphicsPipeline>(new D3D12::D3D12GraphicsPipeline(graphicsContext, pipelineDesc));
	}

}
