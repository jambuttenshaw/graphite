#pragma once

#include "Event.h"
#include "Graphite/Input/KeyCodes.h"


namespace Graphite
{

	class GRAPHITE_API KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(KeyCode keyCode)
			: m_KeyCode(keyCode)
		{}
		virtual ~KeyPressedEvent() = default;

		DEFINE_EVENT_TYPE(KeyPressed, EventCategoryInput | EventCategoryKeyboard);

		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

	private:
		KeyCode m_KeyCode = 0;
	};


	class GRAPHITE_API KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(KeyCode keyCode)
			: m_KeyCode(keyCode)
		{
		}
		virtual ~KeyReleasedEvent() = default;

		DEFINE_EVENT_TYPE(KeyPressed, EventCategoryInput | EventCategoryKeyboard);

		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

	private:
		KeyCode m_KeyCode = 0;
	};

}
