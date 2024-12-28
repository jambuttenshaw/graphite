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


	struct Viewport
	{
		float Left;
		float Top;

		float Width;
		float Height;

		float MinDepth;
		float MaxDepth;
	};

	// General purpose rectangle definition
	struct Rectangle
	{
		int32_t Left;
		int32_t Top;

		int32_t Width;
		int32_t Height;
	};

}
