#include "graphite_d3d12_pch.h"
#include "D3D12GraphicsPipeline.h"

#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/GraphicsContext.h"

#include "RHI/D3D12Exceptions.h"
#include "RHI/D3D12GraphicsContext.h"
#include "RHI/D3D12Types.h"
#include "D3D12ShaderCompiler.h"

namespace Graphite::D3D12
{

	D3D12GraphicsPipeline::D3D12GraphicsPipeline(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& description)
	{
		// Get native device
		ID3D12Device* device = dynamic_cast<const D3D12GraphicsContext&>(graphicsContext).GetDevice();

		// Create root signature
		{
			std::vector<CD3DX12_ROOT_PARAMETER> rootParams;
			std::vector<CD3DX12_DESCRIPTOR_RANGE> descriptorRanges;

			// Build root signature params from resource layout
			if (description.ResourceLayout)
			{
				// To avoid re-allocating the vector and invalidating pointers
				// 8 is the total number of shader stages that a resource can be used in
				// Given a max of 1 range / root param per resource per shader stage... should never re-allocate
				rootParams.reserve(description.ResourceLayout->size() * 8);
				descriptorRanges.reserve(description.ResourceLayout->size() * 8);

				const auto& resources = *description.ResourceLayout;

				for (auto currentBindingFrequency : {
					// Root arguments should preferably be ordered from highest to lowest frequency
					PipelineResourceBindingFrequency::Dynamic,
					PipelineResourceBindingFrequency::Mutable,
					PipelineResourceBindingFrequency::Static
				})
				{
					PipelineResourceSet& resourceSet = GetPipelineResourceSet(currentBindingFrequency);
					resourceSet.SetBaseRootArgumentIndex(static_cast<uint32_t>(rootParams.size()));

					std::map<PipelineResourceShaderVisibility, std::vector<CD3DX12_DESCRIPTOR_RANGE>> descriptorRangesForFrequency;

					auto isCurrentBindingFrequency = [&resources, currentBindingFrequency](int i) -> bool
						{ return resources.at(i).BindingFrequency == currentBindingFrequency; };
					auto getResourceDescription =	 [&resources](size_t i) -> const PipelineResourceDescription& 
						{ return resources.at(i); };

					// Iterate over all resource descriptions at this binding frequency
					for (const PipelineResourceDescription& resourceDescription : std::views::iota(0, static_cast<int>(resources.size()))
																				| std::views::filter(isCurrentBindingFrequency)
																				| std::views::transform(getResourceDescription))
					{
						auto& pipelineResource = resourceSet.AddResource(resourceDescription.ResourceName, resourceDescription);

						// All set bits represent a shader stage this resource is used in
						std::vector<PipelineResourceShaderVisibility> shaderStages;
						if (resourceDescription.ShaderVisibility == ShaderVisibility_All)
						{
							// Visibility_All must be handled uniquely
							shaderStages.push_back(ShaderVisibility_All);
						}
						else
						{
							// Extract all the individual shader stages this resource should be visible to
							std::bitset<sizeof(PipelineResourceShaderVisibility) * 8> visBitset(resourceDescription.ShaderVisibility);
							shaderStages.resize(visBitset.count());

							for (auto& shaderStage : shaderStages)
							{
								int idx = std::countr_zero(visBitset.to_ulong());
								shaderStage = static_cast<PipelineResourceShaderVisibility>(1 << idx);
								visBitset.set(idx, false);
							}
						}

						for (const auto& shaderStage : shaderStages)
						{
							if (resourceDescription.BindingMethod == PipelineResourceBindingMethod::Inline)
							{
								rootParams.emplace_back();
								rootParams.back().ParameterType = GraphiteResourceTypeToD3D12RootParameterType(resourceDescription.Type);
								rootParams.back().Descriptor.ShaderRegister = resourceDescription.BindingSlot;
								rootParams.back().Descriptor.RegisterSpace = resourceDescription.RegisterSpace;
								rootParams.back().ShaderVisibility = GraphiteShaderVisibilityToD3D12ShaderVisibility(shaderStage);

								resourceSet.AddInlineRootArgument(resourceDescription.Type, static_cast<uint32_t>(pipelineResource.DescriptorOffsetOrAddressIndex));
							}
							else if (resourceDescription.BindingMethod == PipelineResourceBindingMethod::Default)
							{
								// Will create an entry if one doesn't already exist
								std::vector<CD3DX12_DESCRIPTOR_RANGE>& ranges = descriptorRangesForFrequency[shaderStage];
								ranges.emplace_back();
								ranges.back().Init(
									GraphiteResourceTypeToD3D12DescriptorRangeType(resourceDescription.Type),
									1,
									resourceDescription.BindingSlot,
									resourceDescription.RegisterSpace,
									static_cast<UINT>(pipelineResource.DescriptorOffsetOrAddressIndex)
								);
							}
							else
							{
								GRAPHITE_ASSERT(false, "Invalid binding method.");
							}
						}
					}

					// Build all descriptor table root arguments
					for (const auto& pair : descriptorRangesForFrequency)
					{
						const auto& visibility = pair.first;
						const auto& ranges = pair.second;

						const size_t descriptorRangeIndex = descriptorRanges.size();
						const size_t descriptorRangeCount = ranges.size();
						descriptorRanges.insert(descriptorRanges.end(), ranges.begin(), ranges.end());

						rootParams.emplace_back();
						rootParams.back().InitAsDescriptorTable(
							static_cast<UINT>(descriptorRangeCount),
							&descriptorRanges.at(descriptorRangeIndex),
							GraphiteShaderVisibilityToD3D12ShaderVisibility(visibility)
						);

						resourceSet.AddDefaultRootArgument();
					}
				}
			}

			D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(static_cast<UINT>(rootParams.size()), rootParams.data(), 0, nullptr, flags);

			ComPtr<ID3DBlob> rootSigBlob, errorBlob;
			HRESULT result = D3D12SerializeRootSignature(
				&rootSignatureDesc,
				D3D_ROOT_SIGNATURE_VERSION_1,
				rootSigBlob.ReleaseAndGetAddressOf(),
				errorBlob.ReleaseAndGetAddressOf());

			if (FAILED(result) || errorBlob != nullptr)
			{
				GRAPHITE_LOG_ERROR("{}", static_cast<char*>(errorBlob->GetBufferPointer()));
				DX_THROW_HR_ERROR(result);
			}

			DX_THROW_IF_FAIL(device->CreateRootSignature(
				0,
				rootSigBlob->GetBufferPointer(),
				rootSigBlob->GetBufferSize(),
				IID_PPV_ARGS(m_RootSignature.ReleaseAndGetAddressOf())
			));
		}

		// Compile shaders
		std::unique_ptr<ShaderBytecode> vs, ps;
		if (description.VertexShader)
		{
			bool success = ShaderCompiler::Get().CompileFromFile(
				description.VertexShader,
				ShaderType::Vertex,
				vs
			);
			GRAPHITE_ASSERT(success, "Failed to compile vertex shader.");
		}

		if (description.PixelShader)
		{
			bool success = ShaderCompiler::Get().CompileFromFile(
				description.PixelShader,
				ShaderType::Pixel,
				ps
			);
			GRAPHITE_ASSERT(success, "Failed to compile pixel shader.");
		}

		// Create pipeline
		{
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.pRootSignature = m_RootSignature.Get();
			psoDesc.VS = dynamic_cast<D3D12ShaderBytecode*>(vs.get())->GetNativeBytecode();
			psoDesc.PS = dynamic_cast<D3D12ShaderBytecode*>(ps.get())->GetNativeBytecode();

			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = false;
			psoDesc.DepthStencilState.StencilEnable = false;

			// Create D3D12 input layout
			std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
			GraphiteInputLayoutToD3D12InputLayout(*description.InputVertexLayout, inputLayout);

			psoDesc.InputLayout = {
				.pInputElementDescs = inputLayout.data(),
				.NumElements = static_cast<UINT>(inputLayout.size())
			};

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
