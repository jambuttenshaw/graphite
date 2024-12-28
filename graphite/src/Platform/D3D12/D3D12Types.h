#pragma once
#include "Graphite/RHI/RHITypes.h"

namespace Graphite
{
	class InputLayout;
}


namespace Graphite::D3D12
{
	extern DXGI_FORMAT GraphiteFormatToD3D12Format(GraphiteFormat format);

	extern D3D12_DESCRIPTOR_HEAP_TYPE GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(GraphiteDescriptorHeapTypes heapType);

	extern void GraphiteInputLayoutToD3D12InputLayout(const InputLayout& inputLayout, std::vector<D3D12_INPUT_ELEMENT_DESC>& outLayout);
}
