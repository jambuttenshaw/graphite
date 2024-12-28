#include "graphite_pch.h"
#include "LayerStack.h"


namespace Graphite
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = 0;
	}

	Layer* LayerStack::PushLayer(std::unique_ptr<Layer> layer)
	{
		LayerIterator it = m_Layers.emplace(begin() + m_LayerInsert, std::move(layer));
		m_LayerInsert++;

		(*it)->OnAttach();
		return it->get();
	}

	Layer* LayerStack::PushOverlay(std::unique_ptr<Layer> overlay)
	{
		Layer* layer = m_Layers.emplace_back(std::move(overlay)).get();
		layer->OnAttach();
		return layer;
	}

	std::unique_ptr<Layer> LayerStack::PopLayer(Layer* layer)
	{
		LayerIterator it = std::find_if(begin(), end(), 
			[&](const std::unique_ptr<Layer>& i) { return i.get() == layer; }
		);
		if (it != end())
		{
			layer->OnDetach();

			auto poppedLayer = std::move(*it);
			m_Layers.erase(it);
			m_LayerInsert--;

			return std::move(poppedLayer);
		}
		return nullptr;
	}

	std::unique_ptr<Layer> LayerStack::PopOverlay(Layer* overlay)
	{
		LayerIterator it = std::find_if(begin(), end(),
			[&](const std::unique_ptr<Layer>& i) { return i.get() == overlay; }
		);
		if (it != end())
		{
			overlay->OnDetach();

			auto poppedLayer = std::move(*it);
			m_Layers.erase(it);

			return std::move(poppedLayer);
		}
		return nullptr;
	}
}
