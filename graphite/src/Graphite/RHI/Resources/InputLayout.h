#pragma once

#include "Graphite/Core/Core.h"
#include "Graphite/RHI/RHITypes.h"


namespace Graphite
{

	enum class VertexAttribute : int8_t
	{
		Invalid = -1,

		Position,
		Normal,
		UV,
		Color,
		Tangent,

		Count
	};


	class InputLayout
	{
	public:
		using InputElementDesc = std::pair<VertexAttribute, GraphiteFormat>;
		struct InputElement
		{
			VertexAttribute Attribute;
			GraphiteFormat Format;
			uint32_t SizeInBytes;
			uint32_t OffsetInBytes;
		};

		using InputElementIterator = std::vector<InputLayout::InputElement>::iterator;
		using InputElementConstIterator = std::vector<InputLayout::InputElement>::const_iterator;

		// Constructs an input layout from a list of elements
		GRAPHITE_API InputLayout(std::initializer_list<InputElementDesc> inputElements, bool interleaved = false);

		GRAPHITE_API inline size_t GetElementCount() const { return m_InputElements.size(); }
		// This would be the size of a single vertex in the layout
		GRAPHITE_API inline uint32_t GetLayoutSizeInBytes() const { return m_LayoutSizeInBytes; }

		GRAPHITE_API bool IsInterleaved() const { return m_Interleaved; }

		GRAPHITE_API bool HasAttribute(VertexAttribute attribute) const;
		GRAPHITE_API const InputElement& GetInputElement(uint32_t index) const { return m_InputElements.at(index); }
		GRAPHITE_API const InputElement& GetInputElement(VertexAttribute attribute) const;

		// Iterators to use InputLayout as a container of input elements
		GRAPHITE_API InputElementIterator begin() { return m_InputElements.begin(); }
		GRAPHITE_API InputElementIterator end() { return m_InputElements.end(); }

		GRAPHITE_API InputElementConstIterator begin() const { return m_InputElements.begin(); }
		GRAPHITE_API InputElementConstIterator end() const { return m_InputElements.end(); }

	private:
		InputElementIterator FindAttribute(VertexAttribute attribute);
		InputElementConstIterator FindAttribute(VertexAttribute attribute) const;

	private:
		std::vector<InputElement> m_InputElements;

		// Size in bytes between the start of each vertex
		uint32_t m_LayoutSizeInBytes;
		bool m_Interleaved;
	};

}
