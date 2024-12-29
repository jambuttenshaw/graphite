#pragma once

#include "Graphite/Core/Core.h"


namespace Graphite
{
	// Input layouts
	struct InputElement
	{
		// Semantics
		std::string SemanticName;
		uint32_t SemanticIndex;

		// Data format
		uint32_t NumChannels;
	};


	class InputLayout
	{
	public:
		// Constructs an input layout from a list of elements
		GRAPHITE_API InputLayout(std::initializer_list<InputElement> inputElements);

		GRAPHITE_API inline size_t GetElementCount() const { return m_InputElements.size(); }
		GRAPHITE_API inline uint32_t GetVertexStride() const { return m_VertexStride; }

		// Iterators to use InputLayout as a container of input elements
		GRAPHITE_API std::vector<InputElement>::iterator begin() { return m_InputElements.begin(); }
		GRAPHITE_API std::vector<InputElement>::iterator end() { return m_InputElements.end(); }

		GRAPHITE_API std::vector<InputElement>::const_iterator begin() const { return m_InputElements.begin(); }
		GRAPHITE_API std::vector<InputElement>::const_iterator end() const { return m_InputElements.end(); }

	private:
		std::vector<InputElement> m_InputElements;

		// Size in bytes between the start of each vertex
		uint32_t m_VertexStride;
	};

}
