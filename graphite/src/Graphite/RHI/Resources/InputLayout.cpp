#include "graphite_pch.h"
#include "InputLayout.h"

#include "Core/Assert.h"


namespace Graphite
{

	InputLayout::InputLayout(std::initializer_list<InputElementDesc> inputElements, bool interleaved)
		: m_Interleaved(interleaved)
	{
		m_LayoutSizeInBytes = 0;
		m_InputElements.reserve(inputElements.size());

		for (const auto& [attribute, format] : inputElements)
		{
			const uint32_t elementSize = GraphiteFormatSizeInBytes(format);
			m_InputElements.emplace_back(
				attribute,
				format,
				elementSize,
				m_LayoutSizeInBytes
			);
			m_LayoutSizeInBytes += elementSize;
		}
	}

	bool InputLayout::HasAttribute(VertexAttribute attribute) const
	{
		return FindAttribute(attribute) != end();
	}

	const InputElement& InputLayout::GetInputElement(VertexAttribute attribute) const
	{
		GRAPHITE_ASSERT(HasAttribute(attribute), "Input layout does not contain this attribute.");
		return *FindAttribute(attribute);
	}

	InputElementIterator InputLayout::FindAttribute(VertexAttribute attribute)
	{
		return std::ranges::find_if(*this, [&](const InputElement& elem) { return elem.Attribute == attribute; });
	}

	InputElementConstIterator InputLayout::FindAttribute(VertexAttribute attribute) const
	{
		return std::ranges::find_if(*this, [&](const InputElement& elem) { return elem.Attribute == attribute; });
	}


}
