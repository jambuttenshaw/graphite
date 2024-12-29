#include "graphite_pch.h"
#include "GraphicsPipeline.h"

#include "Platform/D3D12/GraphiteD3D12.h"


namespace Graphite
{

	std::unique_ptr<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& pipelineDesc)
	{
		return std::unique_ptr<GraphicsPipeline>(D3D12::CreateD3D12GraphicsPipeline(graphicsContext, pipelineDesc));
	}

}
