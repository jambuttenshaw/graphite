#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/RHI/RHITypes.h"


namespace Graphite
{

	struct PipelineResourceDescription
	{
		std::string ResourceName;							// Name that is used to later look up the resource

		PipelineResourceType Type;							// What resource type this object is
		PipelineResourceBindingFrequency BindingFrequency;	// How often this resource will be switched out when using this pipeline
		PipelineResourceBindingMethod BindingMethod;		// How this resource should be bound to the pipeline

		uint32_t BindingSlot;								// The binding slot specified in HLSL 
		uint32_t RegisterSpace;								// The register space specified in HLSL

		PipelineResourceShaderVisibility ShaderVisibility;	// What shaders can access this resource

		// Factories for different type of resources
		GRAPHITE_API static PipelineResourceDescription ConstantBuffer(
			std::string ResourceName,
			PipelineResourceBindingFrequency bindingFrequency,
			PipelineResourceBindingMethod bindingMethod,
			uint32_t bindingSlot,
			uint32_t registerSpace,
			PipelineResourceShaderVisibility shaderVisibility);
	};
}
