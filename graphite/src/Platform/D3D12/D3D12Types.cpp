#include "graphite_pch.h"
#include "D3D12Types.h"

#include "Graphite/Core/Log.h"

#include "Graphite/RHI/Resources/InputLayout.h"


namespace Graphite::D3D12
{
	DXGI_FORMAT GraphiteFormatToD3D12Format(GraphiteFormat format)
	{
		switch (format)
		{
		case GraphiteFormat_R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
		default: break;
		}

		GRAPHITE_LOG_ERROR("Unknown/unsupported Graphite format!");
		return DXGI_FORMAT_UNKNOWN;
	}

	D3D12_DESCRIPTOR_HEAP_TYPE GraphiteDescriptorHeapTypeToD3D12DescriptorHeapType(GraphiteDescriptorHeapTypes heapType)
	{
		switch (heapType)
		{
		case GraphiteDescriptorHeap_RTV:		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case GraphiteDescriptorHeap_DSV:		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		case GraphiteDescriptorHeap_RESOURCE:	return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		case GraphiteDescriptorHeap_SAMPLER:	return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		default: break;
		}

		GRAPHITE_LOG_ERROR("Unknown/unsupported Graphite format!");
		return D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
	}


	static DXGI_FORMAT NumChannelsToFormat(uint32_t numChannels)
	{
		switch (numChannels)
		{
		case 1:
			return DXGI_FORMAT_R32_FLOAT;
		case 2:
			return DXGI_FORMAT_R32G32_FLOAT;
		case 3:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case 4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default:
			GRAPHITE_LOG_ERROR("Invalid number of channels for input element: {}", numChannels);
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	void GraphiteInputLayoutToD3D12InputLayout(const InputLayout& inputLayout, std::vector<D3D12_INPUT_ELEMENT_DESC>& outLayout)
	{
		outLayout.clear();
		outLayout.reserve(inputLayout.GetElementCount());

		// Build a D3D12 layout description out of the input elements
		uint32_t layoutSizeInBytes = 0;
		for (const auto& element : inputLayout)
		{
			// Build D3D12 element desc
			D3D12_INPUT_ELEMENT_DESC desc = {
				.SemanticName = element.SemanticName.c_str(),
				.SemanticIndex = element.SemanticIndex,
				.Format = NumChannelsToFormat(element.NumChannels),
				.InputSlot = 0,
				.AlignedByteOffset = layoutSizeInBytes,
				// Unused
				.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0
			};
			outLayout.push_back(desc);
			layoutSizeInBytes += element.NumChannels * sizeof(uint32_t);
		}
	}


}
