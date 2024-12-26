#include "pch.h"
#include "GraphicsPipeline.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/RHIExceptions.h"
#include "Graphite/RHI/Resources/InputLayout.h"


namespace Graphite
{

	GraphicsPipeline::GraphicsPipeline(ID3D12Device* device, const GraphicsPipelineDescription& description)
	{
		// Create root signature
		{
			D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, flags);

			ComPtr<ID3DBlob> rootSigBlob, errorBlob;
			HRESULT result = D3D12SerializeRootSignature(
				&rootSignatureDesc,
				D3D_ROOT_SIGNATURE_VERSION_1,
				rootSigBlob.ReleaseAndGetAddressOf(),
				errorBlob.ReleaseAndGetAddressOf());

			if (FAILED(result) || errorBlob != nullptr)
			{
				GRAPHITE_LOG_ERROR("{}", static_cast<char*>(errorBlob->GetBufferPointer()));
				//DX_THROW_HR_ERROR(result);
			}

			DX_THROW_IF_FAIL(device->CreateRootSignature(
				0,
				rootSigBlob->GetBufferPointer(),
				rootSigBlob->GetBufferSize(),
				IID_PPV_ARGS(m_RootSignature.ReleaseAndGetAddressOf())
			));
		}

		// Compile shaders
		ShaderBytecode vs, ps;
		if (description.VertexShader)
		{
			vs = ShaderCompiler::Get().CompileFromFile(
				description.VertexShader,
				ShaderType::Vertex
			);
			GRAPHITE_ASSERT(vs.IsValid(), "Failed to compile vertex shader.");
		}

		if (description.VertexShader)
		{
			ps = ShaderCompiler::Get().CompileFromFile(
				description.PixelShader,
				ShaderType::Pixel
			);
			GRAPHITE_ASSERT(ps.IsValid(), "Failed to compile pixel shader.");
		}

		// Create pipeline
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.pRootSignature = m_RootSignature.Get();
			psoDesc.VS = vs.GetBytecode();
			psoDesc.PS = ps.GetBytecode();

			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = false;
			psoDesc.DepthStencilState.StencilEnable = false;

			psoDesc.InputLayout = description.InputVertexLayout->GetLayoutDescription();

			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

			// NOTE: There might be 0 render targets in depth-only passes
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

			psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
			psoDesc.SampleDesc.Count = 1;
			psoDesc.SampleDesc.Quality = 0;

			DX_THROW_IF_FAIL(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState)));
		}
	}
}
