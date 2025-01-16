#include "graphite_pch.h"
#include "GraphicsPipeline.h"

#include "Graphite/Core/Log.h"


namespace Graphite
{

	GraphicsPipeline::GraphicsPipeline()
		: m_StaticResources(PipelineResourceBindingFrequency::Static)
		, m_MutableResources(PipelineResourceBindingFrequency::Mutable)
		, m_DynamicResources(PipelineResourceBindingFrequency::Dynamic)
	{
	}


	const PipelineResourceSet& GraphicsPipeline::GetPipelineResourceSet(PipelineResourceBindingFrequency bindingFrequency) const
	{
		switch (bindingFrequency)
		{
		default: GRAPHITE_LOG_FATAL("Invalid binding frequency!");
		case PipelineResourceBindingFrequency::Static: return m_StaticResources;
		case PipelineResourceBindingFrequency::Mutable: return m_MutableResources;
		case PipelineResourceBindingFrequency::Dynamic: return m_DynamicResources;
		}
	}

	PipelineResourceSet& GraphicsPipeline::GetPipelineResourceSet(PipelineResourceBindingFrequency bindingFrequency)
	{
		switch (bindingFrequency)
		{
		default: GRAPHITE_LOG_FATAL("Invalid binding frequency!");
		case PipelineResourceBindingFrequency::Static: return m_StaticResources;
		case PipelineResourceBindingFrequency::Mutable: return m_MutableResources;
		case PipelineResourceBindingFrequency::Dynamic: return m_DynamicResources;
		}
	}
}
