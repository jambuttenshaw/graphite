#pragma once

#include "Event.h"


namespace Graphite
{

	class GRAPHITE_API KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(int32_t keyCode)
			: m_KeyCode(keyCode)
		{}
		virtual ~KeyPressedEvent() = default;

		DEFINE_EVENT_TYPE(KeyPressed, EventCategoryInput | EventCategoryKeyboard);

		inline int32_t GetKeyCode() const { return m_KeyCode; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode;
			return ss.str();
		}

	private:
		int32_t m_KeyCode = 0;
	};


	class GRAPHITE_API KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(int32_t keyCode)
			: m_KeyCode(keyCode)
		{
		}
		virtual ~KeyReleasedEvent() = default;

		DEFINE_EVENT_TYPE(KeyPressed, EventCategoryInput | EventCategoryKeyboard);

		inline int32_t GetKeyCode() const { return m_KeyCode; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

	private:
		int32_t m_KeyCode = 0;
	};

}
