#include "graphite_pch.h"
#include "PipelineResourceLayout.h"


namespace Graphite
{

	PipelineResourceDescription PipelineResourceDescription::ConstantBuffer(
		std::string ResourceName,
		PipelineResourceBinding binding,
		uint32_t bindingSlot,
		uint32_t registerSpace,
		PipelineResourceShaderVisibility shaderVisibility)
	{
		return PipelineResourceDescription{
			.ResourceName = std::move(ResourceName),
			.Type = PipelineResourceType::ConstantBufferView,
			.Binding = binding,
			.BindingSlot = bindingSlot,
			.RegisterSpace = registerSpace,
			.ShaderVisibility = shaderVisibility
		};
	}


	PipelineResourceLayout::PipelineResourceLayout(std::initializer_list<PipelineResourceDescription> resources)
		: PipelineResources(std::move(resources))
	{
	}

}
