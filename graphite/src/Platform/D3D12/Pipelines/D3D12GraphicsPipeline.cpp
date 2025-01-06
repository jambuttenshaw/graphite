#include "graphite_d3d12_pch.h"
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

	/*
	void BuildDescriptorTables(const std::vector<ResourceTable>& tables, 
		std::vector<CD3DX12_DESCRIPTOR_RANGE>& inOutDescriptorRanges, 
		std::vector<CD3DX12_ROOT_PARAMETER>& outRootParams)
	{
		for (const auto& table : tables)
		{
			// Create descriptor ranges for each type of descriptor in the table
			size_t rangeStartIndex = inOutDescriptorRanges.size();
			uint32_t rangeCount = 0;

			if (table.NumConstantBuffers > 0)
			{
				CD3DX12_DESCRIPTOR_RANGE range;
				range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, table.NumConstantBuffers, 0, 0);
				inOutDescriptorRanges.push_back(range);
				rangeCount++;
			}

			if (table.NumShaderResourceViews > 0)
			{
				CD3DX12_DESCRIPTOR_RANGE range;
				range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, table.NumShaderResourceViews, 0, 0);
				inOutDescriptorRanges.push_back(range);
				rangeCount++;
			}

			if (table.NumUnorderedAccessViews > 0)
			{
				CD3DX12_DESCRIPTOR_RANGE range;
				range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, table.NumUnorderedAccessViews, 0, 0);
				inOutDescriptorRanges.push_back(range);
				rangeCount++;
			}

			CD3DX12_ROOT_PARAMETER rootParam;
			rootParam.InitAsDescriptorTable(rangeCount, &inOutDescriptorRanges.at(rangeStartIndex));
			outRootParams.push_back(rootParam);
		}
	}
	*/

	D3D12GraphicsPipeline::D3D12GraphicsPipeline(const GraphicsContext& graphicsContext, const GraphicsPipelineDescription& description)
	{
		// Get native device
		const D3D12GraphicsContext& nativeContext = dynamic_cast<const D3D12GraphicsContext&>(graphicsContext);
		ID3D12Device* device = nativeContext.GetDevice();

		// Create root signature
		{
			std::vector<CD3DX12_ROOT_PARAMETER> rootParams;
			std::vector<CD3DX12_DESCRIPTOR_RANGE> descriptorRanges;

			if (description.ResourceLayout)
			{
				// Build params for descriptor tables
				const auto& resources = description.ResourceLayout->PipelineResources;
				// We want root signature params ordered from the highest frequency to the lowest frequency
				// so we sort the indices of the resources based on their binding type to access the resources in order
				std::vector<size_t> indices(resources.size());
				std::iota(indices.begin(), indices.end(), 0);

				std::ranges::sort(indices, std::greater<>{}, 
					[&resources](size_t i) -> const PipelineResourceBinding& { return resources.at(i).Binding; });
				auto sortedResources = std::ranges::views::transform(indices, 
					[&resources](size_t i) -> const PipelineResourceDescription& { return resources.at(i); });

				// Keep track of the number of descriptor ranges belonging to each shader visibility
				// A separate root parameter will be created for each visibility, as different flags are required
				// The prefix sum of the counts will give indices into the descriptorRanges vector
				std::map<PipelineResourceShaderVisibility, size_t> descriptorRangeCounts;
				size_t rangeOffset = 0;

				// Every time the binding method changes the descriptor ranges will be built into root params
				PipelineResourceBinding currentBinding = sortedResources[0].Binding;

				auto CreateDescriptorTableParams = [&]()
				{
					for (auto [visibility, count] : descriptorRangeCounts)
					{
						// Create a new root param for each visibility
						CD3DX12_ROOT_PARAMETER rootParam;
						rootParam.InitAsDescriptorTable(
							static_cast<UINT>(count),
							&descriptorRanges[rangeOffset],
							GraphiteShaderVisibilityToD3D12ShaderVisibility(visibility));
						rootParams.push_back(rootParam);

						rangeOffset += count;
					}

					// Counts will reset for next binding type
					// to create separate tables for all parameters with different frequencies
					descriptorRangeCounts.clear();
				};

				for (const PipelineResourceDescription& resource : sortedResources)
				{
					if (resource.Binding != currentBinding)
					{
						CreateDescriptorTableParams();
					}
					currentBinding = resource.Binding;

					if (resource.ShaderVisibility == ShaderVisibility_None)
					{
						GRAPHITE_LOG_WARN("Did you intend to add a resource with ShaderVisibility_None?");
						continue;
					}

					// All set bits represent a shader stage this resource is used in
					std::bitset<sizeof(PipelineResourceShaderVisibility) * 8> shaderVisibilityBits(resource.ShaderVisibility);

					// Add a new resource to the pipeline


					// Add a descriptor for each resource
					for (size_t i = 0; i < shaderVisibilityBits.size(); i++)
					{
						if (!shaderVisibilityBits[i]) continue;
						PipelineResourceShaderVisibility vis = static_cast<PipelineResourceShaderVisibility>(1 << i);

						// TODO: If insertion via operator[] default-inits the value type then this can be turned into one line?
						if (descriptorRangeCounts.contains(vis))
						{
							descriptorRangeCounts.at(vis)++;
						}
						else
						{
							// This shader visibility has not been encountered before - start a new count
							descriptorRangeCounts.insert({ vis, 1 });
						}

						// Create a new descriptor range for this
						CD3DX12_DESCRIPTOR_RANGE descriptorRange;
						descriptorRange.Init(
							GraphiteResourceTypeToD3D12DescriptorRangeType(resource.Type),
							1,
							resource.BindingSlot,
							resource.RegisterSpace
						);
						descriptorRanges.push_back(descriptorRange);
					}
				}

				CreateDescriptorTableParams();
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
