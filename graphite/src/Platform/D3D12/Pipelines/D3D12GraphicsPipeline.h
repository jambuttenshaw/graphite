#pragma once

#include "Graphite/Core/Core.h"

#include "Graphite/RHI/Pipelines/GraphicsPipeline.h"


using Microsoft::WRL::ComPtr;

namespace Graphite
{
	class GraphicsContext;
}


namespace Graphite::D3D12
{
	// Factory function used by core Graphite
	extern GraphicsPipeline* CreateD3D12GraphicsPipeline(const GraphicsContext&, const GraphicsPipelineDescription&);


	class D3D12GraphicsPipeline : public GraphicsPipeline
	{
	public:
		D3D12GraphicsPipeline(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& description);
		virtual ~D3D12GraphicsPipeline() = default;

		DELETE_COPY(D3D12GraphicsPipeline);
		DEFAULT_MOVE(D3D12GraphicsPipeline);

		inline ID3D12RootSignature* GetRootSignature() const { return m_RootSignature.Get(); }
		inline ID3D12PipelineState* GetPipelineState() const { return m_PipelineState.Get(); }

	private:
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;
	};

}
