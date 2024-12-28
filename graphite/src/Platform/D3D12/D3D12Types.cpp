#include "graphite_pch.h"
#include "D3D12Types.h"

#include "Graphite/Core/Log.h"


namespace Graphite::D3D12
{
	DXGI_FORMAT GraphiteFormatToD3D12Format(GRAPHITE_FORMAT format)
	{
		switch (format)
		{
		case GRAPHITE_FORMAT_R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
		default: break;
		}

		GRAPHITE_LOG_ERROR("Unknown/unsupported Graphite format!");
		return DXGI_FORMAT_UNKNOWN;
	}

	D3D12_DESCRIPTOR_HEAP_TYPE GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(GRAPHITE_DESCRIPTOR_HEAP_TYPES heapType)
	{
		switch (heapType)
		{
		case GRAPHITE_DESCRIPTOR_HEAP_RTV:		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case GRAPHITE_DESCRIPTOR_HEAP_DSV:		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		case GRAPHITE_DESCRIPTOR_HEAP_RESOURCE:	return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		case GRAPHITE_DESCRIPTOR_HEAP_SAMPLER:	return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		default: break;
		}

		GRAPHITE_LOG_ERROR("Unknown/unsupported Graphite format!");
		return D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
	}


}
