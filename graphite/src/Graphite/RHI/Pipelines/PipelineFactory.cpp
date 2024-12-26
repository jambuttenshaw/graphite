#include "pch.h"
#include "PipelineFactory.h"

#include "Graphite/Core/Assert.h"


namespace Graphite
{

	PipelineFactory& PipelineFactory::Get()
	{
		static PipelineFactory Instance;
		return Instance;
	}


	void PipelineFactory::Initialize(ID3D12Device* device)
	{
		m_Device = device;
	}


	std::unique_ptr<GraphicsPipeline> PipelineFactory::CreateGraphicsPipeline(const GraphicsPipelineDescription& description)
	{
		GRAPHITE_ASSERT(m_Device, "Pipeline factory has not been initialized by the graphics context!");

		return std::unique_ptr<GraphicsPipeline>(new GraphicsPipeline(m_Device, description));
	}

}
