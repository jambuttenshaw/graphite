#pragma once


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
		InputLayout(std::initializer_list<InputElement> inputElements);

		// InputLayout object lifetime must exceed the lifetime of the returned layout description
		D3D12_INPUT_LAYOUT_DESC GetLayoutDescription() const;
		inline uint32_t GetVertexStride() const { return m_VertexStride; }

	private:
		// Helpers
		static DXGI_FORMAT NumChannelsToFormat(uint32_t numChannels);

	private:
		std::vector<InputElement> m_InputElements;
		// The input layout as expected by the graphics API
		std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

		// Size in bytes between the start of each vertex
		uint32_t m_VertexStride;
	};

}
