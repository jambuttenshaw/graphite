#pragma once
#include "Graphite/Core/Core.h"


namespace Graphite
{
	class Event;
	class RendererInterface;

	class GRAPHITE_API Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = 0;

		DEFAULT_COPY(Layer);
		DEFAULT_MOVE(Layer);

		// Virtual interface to be implemented by layer clients
		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate() {}

		// Rendering interface
		// If a layer does not wish to perform rendering it may return null optional
		virtual std::optional<RendererInterface&> GetRenderer() { return std::nullopt; }

		// Event integration
		// Layers can respond to application events
		virtual void OnEvent(Event&) {}
	};

}
