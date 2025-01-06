#pragma once

#include "Graphite/Core/Core.h"
#include "PipelineResourceLayout.h"


namespace Graphite
{


	// Corresponds to a single ResourceTable's worth of descriptors
	// Resource view lists should be built offline as much as possible
	// Creating a resource view list once and using it in multiple pipelines/over multiple frames
	// is cheaper than continually constructing new lists
	class ResourceViewList
	{
	public:
		// Constructs an invalid resource view list
		GRAPHITE_API ResourceViewList() = default;
		// Create a resource view list to populate a specific table for a pipeline
		//GRAPHITE_API ResourceViewList(const ResourceTable& table);

		//GRAPHITE_API void SetConstantBufferView(uint32_t index, const ConstantBuffer& constantBuffer);

		// Get a handle to the start of the resource view list
		GRAPHITE_API GPUDescriptorHandle GetHandle() const;

	private:
		// One allocation for each frame in flight
		// This is to allow the lifetime of a resource view list to be more than one frame
		// This handles both changing resources in the list dynamically
		// and for buffered resources (such as constant buffers being updated by the CPU)
		//std::vector<DescriptorAllocation> m_DescriptorAllocations;
	};


	class PipelineResourceBinder
	{
	public:
		PipelineResourceBinder(PipelineResourceLayout& pipelineResourceLayout);

		void SetMutableResources(const ResourceViewList& resourceViewList);
		void SetDynamicResources(const ResourceViewList& resourceViewList);

	private:
		// Handles to the beginning of the tables
		GPUDescriptorHandle m_MutableResources;
		GPUDescriptorHandle m_DynamicResources;
	};
}
