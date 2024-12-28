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
	extern DXGI_FORMAT GraphiteFormatToD3D12Format(GraphiteFormat format);
	extern D3D_PRIMITIVE_TOPOLOGY GraphiteTopologyToD3D12Topology(GraphiteTopology topology);

	extern D3D12_DESCRIPTOR_HEAP_TYPE GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(GraphiteDescriptorHeapTypes heapType);

	extern D3D12_VIEWPORT GraphiteViewportToD3D12Viewport(const Viewport& viewport);

	extern void GraphiteInputLayoutToD3D12InputLayout(const InputLayout& inputLayout, std::vector<D3D12_INPUT_ELEMENT_DESC>& outLayout);


	extern D3D12_VERTEX_BUFFER_VIEW GraphiteVBVToD3D12VBV(const VertexBufferView& vbv);
	extern D3D12_INDEX_BUFFER_VIEW GraphiteIBVToD3D12IBV(const IndexBufferView& vbv);
}
