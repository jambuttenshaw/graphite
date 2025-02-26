#include "graphite_pch.h"
#include "InputLayout.h"


namespace Graphite
{

	InputLayout::InputLayout(std::initializer_list<InputElement> inputElements)
	{
		m_InputElements = { inputElements };

		m_VertexStride = 0;
		for (const auto& element : m_InputElements)
		{
			m_VertexStride += element.NumChannels * sizeof(float);
		}
	}

	void InputLayout::AddInputElement(InputElement&& inputElement)
	{
		m_VertexStride += inputElement.NumChannels * sizeof(float);
		m_InputElements.emplace_back(std::move(inputElement));
	}

}
