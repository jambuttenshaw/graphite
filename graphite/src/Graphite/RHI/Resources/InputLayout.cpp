#include "pch.h"
#include "InputLayout.h"

#include "Graphite/Core/Log.h"


namespace Graphite
{

	InputLayout::InputLayout(std::initializer_list<InputElement> inputElements)
	{
		m_InputElements.reserve(inputElements.size());
		m_InputLayout.reserve(inputElements.size());

		for (const auto& element : inputElements)
		{
			m_InputElements.push_back(element);
		}

		// Build a D3D12 layout description out of the input elements
		uint32_t layoutSizeInBytes = 0;
		for (const auto& element : m_InputElements)
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
			layoutSizeInBytes += element.NumChannels * sizeof(uint32_t);
		}

		m_VertexStride = layoutSizeInBytes;
	}

	D3D12_INPUT_LAYOUT_DESC InputLayout::GetLayoutDescription() const
	{
		return D3D12_INPUT_LAYOUT_DESC{
			.pInputElementDescs = m_InputLayout.data(),
			.NumElements = static_cast<UINT>(m_InputLayout.size())
		};
	}



	DXGI_FORMAT InputLayout::NumChannelsToFormat(uint32_t numChannels)
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


}
