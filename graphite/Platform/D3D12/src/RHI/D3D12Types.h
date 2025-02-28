#pragma once
#include "Graphite/RHI/RHITypes.h"

namespace Graphite
{
	class InputLayout;

	struct VertexBufferView;
	struct IndexBufferView;
}


namespace Graphite::D3D12
{
	extern D3D12_GPU_VIRTUAL_ADDRESS GraphiteGPUAddressToD3D12GPUAddress(GPUVirtualAddress address);

	extern DXGI_FORMAT GraphiteFormatToD3D12Format(GraphiteFormat format);
	extern D3D_PRIMITIVE_TOPOLOGY GraphiteTopologyToD3D12Topology(GraphiteTopology topology);

	extern D3D12_DESCRIPTOR_HEAP_TYPE GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(DescriptorHeapType heapType);

	extern D3D12_CPU_DESCRIPTOR_HANDLE GraphiteCPUDescriptorToD3D12Descriptor(CPUDescriptorHandle descriptor);
	extern CPUDescriptorHandle D3D12CPUDescriptorToGraphiteDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE descriptor);

	extern D3D12_GPU_DESCRIPTOR_HANDLE GraphiteGPUDescriptorToD3D12Descriptor(GPUDescriptorHandle descriptor);
	extern GPUDescriptorHandle D3D12GPUDescriptorToGraphiteDescriptor(D3D12_GPU_DESCRIPTOR_HANDLE descriptor);

	extern D3D12_VIEWPORT GraphiteViewportToD3D12Viewport(const Viewport& viewport);

	extern void GraphiteInputLayoutToD3D12InputLayout(const InputLayout& inputLayout, std::vector<D3D12_INPUT_ELEMENT_DESC>& outLayout);


	extern D3D12_VERTEX_BUFFER_VIEW GraphiteVBVToD3D12VBV(const VertexBufferView& vbv);
	extern D3D12_INDEX_BUFFER_VIEW GraphiteIBVToD3D12IBV(const IndexBufferView& ibv);

	extern D3D12_DESCRIPTOR_RANGE_TYPE GraphiteResourceTypeToD3D12DescriptorRangeType(PipelineResourceType type);
	extern D3D12_ROOT_PARAMETER_TYPE GraphiteResourceTypeToD3D12RootParameterType(PipelineResourceType type);

	extern D3D12_SHADER_VISIBILITY GraphiteShaderVisibilityToD3D12ShaderVisibility(PipelineResourceShaderVisibility visibility);
}
