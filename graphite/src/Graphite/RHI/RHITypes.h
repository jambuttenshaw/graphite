#pragma once


namespace Graphite
{
	// Common RHI types
	// To translate from API-specific types to Graphite-specific types

	using GraphiteGPUVirtualAddress = uint64_t;


	enum GraphiteFormat
	{
		GraphiteFormat_R8G8B8A8_UNORM
	};


	enum GraphiteDescriptorHeapTypes
	{
		GraphiteDescriptorHeap_RTV,
		GraphiteDescriptorHeap_DSV,
		GraphiteDescriptorHeap_RESOURCE,
		GraphiteDescriptorHeap_SAMPLER
	};

}
