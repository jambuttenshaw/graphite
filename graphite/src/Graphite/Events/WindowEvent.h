#pragma once

#include "Event.h"


namespace Graphite
{

	class GRAPHITE_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
		virtual ~WindowCloseEvent() = default;

		DEFINE_EVENT_TYPE(WindowClose, EventCategoryWindow);
	};
}
