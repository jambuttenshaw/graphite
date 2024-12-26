#pragma once

#include "Graphite/Core/Core.h"

#include "ShaderCompiler.h"


using Microsoft::WRL::ComPtr;


namespace Graphite
{

	class InputLayout;

	struct GraphicsPipelineDescription
	{
		const InputLayout* InputVertexLayout;

		ShaderDescription VertexShader;
		ShaderDescription PixelShader;
	};


	class GraphicsPipeline
	{
	protected:
		friend class PipelineFactory;
		GraphicsPipeline(ID3D12Device* device, const GraphicsPipelineDescription& description);
	public:
		~GraphicsPipeline() = default;

		DELETE_COPY(GraphicsPipeline);
		DEFAULT_MOVE(GraphicsPipeline);

		inline ID3D12RootSignature* GetRootSignature() const { return m_RootSignature.Get(); }
		inline ID3D12PipelineState* GetPipelineState() const { return m_PipelineState.Get(); }

	private:
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;
	};

}
