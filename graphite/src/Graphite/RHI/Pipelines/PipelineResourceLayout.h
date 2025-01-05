#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/RHI/DescriptorHeap.h"


namespace Graphite
{

	// A description of all the resources used by a pipeline
	// Resources are arranged into separate tables that match with the frequency that data changes
	// e.g., all data that will be constant throughout a frame will be placed into one table
	// All data that is constant 'per material' will be placed in a different table
	// This is to minimize the duplication of descriptors in the resource heap
	class PipelineResourceLayout
	{
	public:
		virtual ~PipelineResourceLayout() = default;

		// TODO: Perhaps allow copies to allow pipelines requiring the same layout to share a root signature object
		DELETE_COPY(PipelineResourceLayout);
		DEFAULT_MOVE(PipelineResourceLayout);

		// Manually add resources to the layout
		void AddTable();

		void AddConstantBufferView(uint32_t table);
		void AddShaderResourceView(uint32_t table);
		void AddUnorderedAccessView(uint32_t table);


		// API implemented
		// Takes the described layout and builds it into a signature that can be used by the API
		// Once the layout is built it can no longer be modified
		virtual void BuildLayout() = 0;

	protected:

	};


	// An instance of a pipeline resource layout
	// This contains the descriptors referencing the actual resources to be used in the pipeline
	class PipelineResourceList
	{
	public:
		virtual ~PipelineResourceList() = default;

	protected:
		
	};

}
