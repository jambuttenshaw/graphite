#include "graphite_pch.h"
#include "InputLayout.h"

#include "Core/Assert.h"


namespace Graphite
{

	InputLayout::InputLayout(std::initializer_list<InputElement> inputElements, bool interleaved)
		: m_Interleaved(interleaved)
	{
		m_InputElements = { inputElements };
		m_ElementOffsets.reserve(m_InputElements.size());

		m_LayoutSizeInBytes = 0;
		for (const auto& format : inputElements | std::views::values)
		{
			m_ElementOffsets.push_back(m_LayoutSizeInBytes);
			m_LayoutSizeInBytes += GraphiteFormatSizeInBytes(format);
		}
	}

	bool InputLayout::HasAttribute(VertexAttribute attribute) const
	{
		return FindAttribute(attribute) != end();
	}

	size_t InputLayout::GetAttributeIndex(VertexAttribute attribute) const
	{
		GRAPHITE_ASSERT(HasAttribute(attribute), "Input layout does not contain this attribute.");
		return std::distance(begin(), FindAttribute(attribute));
	}

	GraphiteFormat InputLayout::GetAttributeFormat(VertexAttribute attribute) const
	{
		GRAPHITE_ASSERT(HasAttribute(attribute), "Input layout does not contain this attribute.");
		return FindAttribute(attribute)->second;
	}

	InputLayout::InputElementContainer::iterator InputLayout::FindAttribute(VertexAttribute attribute)
	{
		return std::ranges::find_if(*this, [&](const InputElement& elem) { return elem.first == attribute; });
	}

	InputLayout::InputElementContainer::const_iterator InputLayout::FindAttribute(VertexAttribute attribute) const
	{
		return std::ranges::find_if(*this, [&](const InputElement& elem) { return elem.first == attribute; });
	}


}
