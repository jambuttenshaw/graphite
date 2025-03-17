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
	extern D3D12_GPU_VIRTUAL_ADDRESS ToD3D12GPUAddress(GPUVirtualAddress address);

	extern DXGI_FORMAT ToD3D12Format(GraphiteFormat format);
	extern D3D12_RESOURCE_STATES ToD3D12ResourceState(ResourceState state);
	extern D3D_PRIMITIVE_TOPOLOGY ToD3D12Topology(GraphiteTopology topology);

	extern D3D12_DESCRIPTOR_HEAP_TYPE ToD3D12DescriptorHeapType(DescriptorHeapType heapType);

	extern D3D12_CPU_DESCRIPTOR_HANDLE ToD3D12CpuDescriptor(CPUDescriptorHandle descriptor);
	extern CPUDescriptorHandle ToGraphiteDescriptor(D3D12_CPU_DESCRIPTOR_HANDLE descriptor);

	extern D3D12_GPU_DESCRIPTOR_HANDLE ToD3D12GpuDescriptor(GPUDescriptorHandle descriptor);
	extern GPUDescriptorHandle ToGraphiteDescriptor(D3D12_GPU_DESCRIPTOR_HANDLE descriptor);

	extern D3D12_VIEWPORT ToD3D12Viewport(const Viewport& viewport);

	extern void ToD3D12InputLayout(const InputLayout& inputLayout, std::vector<D3D12_INPUT_ELEMENT_DESC>& outLayout);


	extern D3D12_VERTEX_BUFFER_VIEW ToD3D12VBV(const VertexBufferView& vbv);
	extern D3D12_INDEX_BUFFER_VIEW ToD3D12IBV(const IndexBufferView& ibv);

	extern D3D12_DESCRIPTOR_RANGE_TYPE ToD3D12DescriptorRangeType(PipelineResourceType type);
	extern D3D12_ROOT_PARAMETER_TYPE ToD3D12RootParameterType(PipelineResourceType type);

	extern D3D12_SHADER_VISIBILITY ToD3D12ShaderVisibility(PipelineResourceShaderVisibility visibility);
}
