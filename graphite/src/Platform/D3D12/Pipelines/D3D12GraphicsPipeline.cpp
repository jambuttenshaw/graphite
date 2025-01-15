#include "graphite_pch.h"
#include "D3D12GraphicsPipeline.h"

#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/GraphicsContext.h"

#include "Platform/D3D12/D3D12Exceptions.h"
#include "Platform/D3D12/D3D12GraphicsContext.h"
#include "D3D12ShaderCompiler.h"
#include "Platform/D3D12/D3D12Types.h"

namespace Graphite::D3D12
{
	GraphicsPipeline* CreateD3D12GraphicsPipeline(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& description)
	{
		return new D3D12GraphicsPipeline(graphicsContext, description);
	}


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
				// Build params for descriptor tables
				const auto& resources = description.ResourceLayout->PipelineResources;
				// We want root signature params ordered from the highest frequency to the lowest frequency
				// so we sort the indices of the resources based on their binding type to access the resources in order
				std::vector<size_t> indices(resources.size());
				std::iota(indices.begin(), indices.end(), 0);

				std::ranges::sort(indices, std::greater<>{}, 
					[&resources](size_t i) -> const PipelineResourceBindingFrequency& { return resources.at(i).BindingFrequency; });
				auto sortedResources = std::ranges::views::transform(indices, 
					[&resources](size_t i) -> const PipelineResourceDescription& { return resources.at(i); });

				// Store data to create root params later, to avoid memory being invalidated when descriptorRanges resizes
				struct RootParamData
				{
					PipelineResourceBindingFrequency BindingFrequency;
					PipelineResourceBindingMethod BindingMethod;
					D3D12_SHADER_VISIBILITY Visibility;

					union
					{
						struct
						{
							uint32_t RangeCount;
							uint32_t RangeIndex;
							uint32_t DescriptorOffsetInResourceSet;	// Offset in descriptors from the start of the resource set
						} DefaultBindingData;
						struct
						{
							size_t PipelineResourceIndex;
							size_t InlineResourceIndex;
						} InlineBindingData;
					};
				};
				std::vector<RootParamData> rootParamData;

				// Keep track of the number of descriptor ranges belonging to each shader visibility
				// A separate root parameter will be created for each visibility, as different flags are required
				// The prefix sum of the counts will give indices into the descriptorRanges vector
				struct DescriptorRangeData
				{
					size_t DescriptorRangeCount = 0;

					// Need to keep track of which resources have their descriptor indices in this root param
					// so that the correct offset into the descriptor table can be set

					// This vector keeps track of resources that will have a descriptor in this table
					// The resource needs to know where in the table its descriptor will lie
					std::vector<
						std::pair</* PipelineResource idx */size_t, /* Bind point idx */size_t>
					> ResourceDescriptorBindings;
				};

				// A separate descriptor table will be built for each shader visibility,
				// because a different shader visibility requires different root parameter flags
				std::map<PipelineResourceShaderVisibility, DescriptorRangeData> descriptorTablesToBuild;
				// When populating the descriptor params, we need to refer back to properties of the resources
				// We keep track of the resources IN ORDER THEY WERE PROCESSED - so we don't need to constantly look up by name
				std::vector<PipelineResource*> pipelineResourceRefs;

				size_t totalRangeOffset = 0;
				size_t inlineResourceCount = 0;

				// Every time the binding method changes the descriptor ranges will be built into root params
				PipelineResourceBindingFrequency currentBindingFrequency = sortedResources[0].BindingFrequency;

				auto CreateDescriptorTableParams = [&]()
				{
					// For prefix sum of 
					size_t rangeOffset = 0;

					// Each shader visibility type will have its own root parameter
					for (const auto& data : descriptorTablesToBuild)
					{
						// Create a new root param for each visibility
						rootParamData.emplace_back();
						rootParamData.back().BindingFrequency = currentBindingFrequency;
						rootParamData.back().BindingMethod = PipelineResourceBindingMethod::Default;
						rootParamData.back().Visibility = GraphiteShaderVisibilityToD3D12ShaderVisibility(data.first);

						rootParamData.back().DefaultBindingData.RangeCount = static_cast<uint32_t>(data.second.DescriptorRangeCount);
						rootParamData.back().DefaultBindingData.RangeIndex = static_cast<uint32_t>(totalRangeOffset + rangeOffset);
						rootParamData.back().DefaultBindingData.DescriptorOffsetInResourceSet = static_cast<uint32_t>(rangeOffset);

						// Apply range offset to all resource binding points
						for (const auto& binding : data.second.ResourceDescriptorBindings)
						{
							auto resourceRef = pipelineResourceRefs.at(binding.first);
							resourceRef->BindPoints.at(binding.second) += rangeOffset;

							// Create a new descriptor range for this
							// We do this now to ensure that all ranges in the table are contiguous in the descriptorRanges vector
							descriptorRanges.emplace_back();
							descriptorRanges.back().Init(
								GraphiteResourceTypeToD3D12DescriptorRangeType(resourceRef->Description.Type),
								1,
								resourceRef->Description.BindingSlot,
								resourceRef->Description.RegisterSpace
							);
						}

						rangeOffset += data.second.DescriptorRangeCount;
					}

					totalRangeOffset += rangeOffset;

					// Counts will reset for next binding type
					// to create separate tables for all parameters with different frequencies
					descriptorTablesToBuild.clear();
				};

				// Iterate over all resources in the layout
				for (const PipelineResourceDescription& resourceDescription : sortedResources)
				{
					GRAPHITE_ASSERT(resourceDescription.BindingFrequency != PipelineResourceBindingFrequency::Invalid, "Invalid binding frequency.");
					GRAPHITE_ASSERT(resourceDescription.BindingMethod != PipelineResourceBindingMethod::Invalid, "Invalid binding method.");
					GRAPHITE_ASSERT(resourceDescription.ShaderVisibility != ShaderVisibility_None, "Invalid shader visibility.");

					// Resource binding frequency is monotonically decreasing as we iterate over the range
					// When the binding frequency changes, we need to consolidate all root parameters for the previous binding frequency
					if (resourceDescription.BindingFrequency != currentBindingFrequency)
					{
						inlineResourceCount = 0;

						CreateDescriptorTableParams();

						currentBindingFrequency = resourceDescription.BindingFrequency;
						// Now that the binding frequency has switched, we need to keep track of where the root parameters for this resource set begin
						GetPipelineResourceSet(currentBindingFrequency).SetBaseRootArgumentIndex(static_cast<uint32_t>(rootParamData.size()));
					}

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

					// Create a new description of the resource for the pipeline to hold on to,
					// so that it knows how to set and bind it
					PipelineResource pipelineResource{
						resourceDescription, // Copy description
						std::vector<size_t>(shaderStages.size())
					};

					// Add a descriptor for each resource
					size_t resourceBindPoint = 0; // There will be a bind point for each stage the resource is used
					for (auto& stage : shaderStages)
					{
						if (resourceDescription.BindingMethod == PipelineResourceBindingMethod::Default)
						{
							size_t currentRangeInArgCount = 0;
							
							if (descriptorTablesToBuild.contains(stage))
							{
								DescriptorRangeData& data = descriptorTablesToBuild.at(stage);
								currentRangeInArgCount = data.DescriptorRangeCount++;
								data.ResourceDescriptorBindings.emplace_back(std::make_pair( pipelineResourceRefs.size(), resourceBindPoint ));
							}
							else
							{
								descriptorTablesToBuild.insert(std::make_pair(stage,
									DescriptorRangeData{
										1,
										{ { pipelineResourceRefs.size(), resourceBindPoint } }
									}));
							}

							// The range offset of the root param must be added to this
							pipelineResource.BindPoints[resourceBindPoint] = currentRangeInArgCount;
							// Next iteration will use a different bind point
							resourceBindPoint++;
						}
						else
						{
							// Inlined resources
							// We can directly specify the root parameter to create here
							rootParamData.emplace_back();
							rootParamData.back().BindingFrequency = currentBindingFrequency;
							rootParamData.back().BindingMethod = PipelineResourceBindingMethod::Inline;
							rootParamData.back().Visibility = GraphiteShaderVisibilityToD3D12ShaderVisibility(stage);
							rootParamData.back().InlineBindingData.PipelineResourceIndex = pipelineResourceRefs.size();
							rootParamData.back().InlineBindingData.InlineResourceIndex = inlineResourceCount;

							pipelineResource.BindPoints[resourceBindPoint] = inlineResourceCount;

							inlineResourceCount++;
						}
					}

					// Add the new resource to the pipeline
					PipelineResourceSet& resourceSet = GetPipelineResourceSet(currentBindingFrequency);
					// Keep temporary track of resources to adjust their bind points once we know the final offsets in the descriptor table
					// This can only be done when all resources of the same binding frequency have been processed
					resourceSet.AddResource(resourceDescription.ResourceName, std::move(pipelineResource));
					pipelineResourceRefs.push_back(&resourceSet.GetResource(resourceDescription.ResourceName));
				}

				CreateDescriptorTableParams();

				// Now that types and properties of all required root parameters have been identified,
				// the actual root parameters can be constructed
				for (const auto& param : rootParamData)
				{
					PipelineResourceSet& resourceSet = GetPipelineResourceSet(param.BindingFrequency);

					rootParams.emplace_back();
					CD3DX12_ROOT_PARAMETER& rootParam = rootParams.back();

					switch (param.BindingMethod)
					{
					case PipelineResourceBindingMethod::Default:
					{
						rootParam.InitAsDescriptorTable(
							param.DefaultBindingData.RangeCount,
							&descriptorRanges[param.DefaultBindingData.RangeIndex],
							param.Visibility);

						resourceSet.AddDefaultRootArgument(param.DefaultBindingData.DescriptorOffsetInResourceSet);
						break;
					}
					case PipelineResourceBindingMethod::Inline:
					{
						const auto& desc = pipelineResourceRefs.at(param.InlineBindingData.PipelineResourceIndex);
							switch (desc->Description.Type)
							{
							case PipelineResourceType::ConstantBufferView:
								rootParam.InitAsConstantBufferView(
									desc->Description.BindingSlot,
									desc->Description.RegisterSpace,
									param.Visibility);
								break;
							case PipelineResourceType::ShaderResourceView:
								rootParam.InitAsShaderResourceView(
									desc->Description.BindingSlot,
									desc->Description.RegisterSpace,
									param.Visibility);
								break;
							case PipelineResourceType::UnorderedAccessView:
								rootParam.InitAsUnorderedAccessView(
									desc->Description.BindingSlot,
									desc->Description.RegisterSpace,
									param.Visibility);
								break;
							default:
								GRAPHITE_LOG_FATAL("Unknown / Unimplemented resource type encountered.");
							}

						resourceSet.AddInlineRootArgument(desc->Description.Type, static_cast<uint32_t>(param.InlineBindingData.InlineResourceIndex));
						break;
					} 
					default:
						GRAPHITE_LOG_FATAL("Invalid resource binding method has been encountered!");
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
