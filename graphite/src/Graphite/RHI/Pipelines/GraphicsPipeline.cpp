#include "graphite_pch.h"
#include "GraphicsPipeline.h"

namespace Graphite::D3D12
{
	extern GraphicsPipeline* CreateD3D12GraphicsPipeline(const GraphicsContext&, const GraphicsPipelineDescription&);
}


namespace Graphite
{

	std::unique_ptr<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& pipelineDesc)
	{
		return std::unique_ptr<GraphicsPipeline>(D3D12::CreateD3D12GraphicsPipeline(graphicsContext, pipelineDesc));
	}

}
