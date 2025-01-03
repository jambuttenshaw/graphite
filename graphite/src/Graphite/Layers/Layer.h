#pragma once
#include "Graphite/Core/Core.h"


namespace Graphite
{
	class Event;

	class GRAPHITE_API Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		DEFAULT_COPY(Layer);
		DEFAULT_MOVE(Layer);

		// Virtual interface to be implemented by layer clients
		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate() {}
		virtual void OnRender() {}

		virtual void OnEvent(Event&) {}
	};

}
