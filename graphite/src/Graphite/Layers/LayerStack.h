#pragma once

#include "Layer.h"


namespace Graphite
{

	class LayerStack
	{
	public:
		using LayerIterator = std::vector<std::unique_ptr<Layer>>::iterator;

	public:
		LayerStack();
		~LayerStack();

		DELETE_COPY(LayerStack);
		DEFAULT_MOVE(LayerStack);

		Layer* PushLayer(std::unique_ptr<Layer> layer);
		Layer* PushOverlay(std::unique_ptr<Layer> overlay);

		std::unique_ptr<Layer> PopLayer(Layer* layer);
		std::unique_ptr<Layer> PopOverlay(Layer* overlay);

		LayerIterator begin() { return m_Layers.begin(); }
		LayerIterator end() { return m_Layers.end(); }

		void Clear();

	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		// Offset into layer stack dividing layers and overlays
		size_t m_LayerInsert;
	};

}
