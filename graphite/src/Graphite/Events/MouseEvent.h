#pragma once

#include "Event.h"
#include "Graphite/Input/KeyCodes.h"


namespace Graphite
{
	class GRAPHITE_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(uint32_t mouseX, uint32_t mouseY)
			: m_MouseX(mouseX)
			, m_MouseY(mouseY)
		{}
		virtual ~MouseMovedEvent() = default;

		DEFINE_EVENT_TYPE(MouseMoved, EventCategoryInput | EventCategoryMouse);

		inline uint32_t GetMouseX() const { return m_MouseX; }
		inline uint32_t GetMouseY() const { return m_MouseY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_MouseX << ", " << m_MouseY << ")";
			return ss.str();
		}

	protected:
		uint32_t m_MouseX = static_cast<uint32_t>(-1);
		uint32_t m_MouseY = static_cast<uint32_t>(-1);
	};


	class GRAPHITE_API MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(KeyCode mouseButton)
			: m_MouseButton(mouseButton)
		{}
		virtual ~MouseButtonPressedEvent() = default;

		DEFINE_EVENT_TYPE(MouseButtonPressed, EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);

		inline KeyCode GetMouseButton() const { return m_MouseButton; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_MouseButton;
			return ss.str();
		}

	private:
		KeyCode m_MouseButton = 0;
	};


	class GRAPHITE_API MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(KeyCode mouseButton)
			: m_MouseButton(mouseButton)
		{
		}
		virtual ~MouseButtonReleasedEvent() = default;

		DEFINE_EVENT_TYPE(MouseButtonReleased, EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);

		inline KeyCode GetMouseButton() const { return m_MouseButton; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_MouseButton;
			return ss.str();
		}

	private:
		KeyCode m_MouseButton = 0;
	};


	class GRAPHITE_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(int32_t scroll)
			: m_Scroll(scroll)
		{}
		virtual ~MouseScrolledEvent() = default;

		DEFINE_EVENT_TYPE(MouseScrolled, EventCategoryInput | EventCategoryMouse);

		inline uint32_t GetScroll() const { return m_Scroll; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_Scroll << ")";
			return ss.str();
		}

	protected:
		int32_t m_Scroll = 0;
	};
}
