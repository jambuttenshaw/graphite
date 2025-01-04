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

		// Any rendering performed in a layer should be performed in at least one pass
		// I.e., a pair of BeginPass()/EndPass() should be called within the OnRender function
		virtual void OnRender() {}

		virtual void OnEvent(Event&) {}
	};

}
