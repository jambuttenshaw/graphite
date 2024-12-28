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
	class D3D12GraphicsPipeline : public GraphicsPipeline
	{
	protected:
		friend class GraphicsPipeline;
		D3D12GraphicsPipeline(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& description);
	public:
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
