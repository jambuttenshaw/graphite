#pragma once

#include "GraphicsPipeline.h"


namespace Graphite
{
	class PipelineFactory
	{
		PipelineFactory() = default;
	public:

		static PipelineFactory& Get();

		// Initialization must be called by the graphics context before this can be used
		void Initialize(ID3D12Device* device);


		// Pipeline factory functions
		std::unique_ptr<GraphicsPipeline> CreateGraphicsPipeline(const GraphicsPipelineDescription& description);

	private:
		ID3D12Device* m_Device = nullptr;
	};
}
