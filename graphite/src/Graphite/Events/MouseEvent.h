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
		MouseButtonPressedEvent(MouseButton mouseButton)
			: m_MouseButton(mouseButton)
		{}
		virtual ~MouseButtonPressedEvent() = default;

		DEFINE_EVENT_TYPE(MouseButtonPressed, EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);

		inline MouseButton GetMouseButton() const { return m_MouseButton; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << static_cast<uint32_t>(m_MouseButton);
			return ss.str();
		}

	private:
		MouseButton m_MouseButton = MouseButton_None;
	};


	class GRAPHITE_API MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(MouseButton mouseButton)
			: m_MouseButton(mouseButton)
		{
		}
		virtual ~MouseButtonReleasedEvent() = default;

		DEFINE_EVENT_TYPE(MouseButtonReleased, EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton);

		inline MouseButton GetMouseButton() const { return m_MouseButton; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << static_cast<uint32_t>(m_MouseButton);
			return ss.str();
		}

	private:
		MouseButton m_MouseButton = MouseButton_None;
	};


	class GRAPHITE_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float scroll)
			: m_Scroll(scroll)
		{}
		virtual ~MouseScrolledEvent() = default;

		DEFINE_EVENT_TYPE(MouseScrolled, EventCategoryInput | EventCategoryMouse);

		inline float GetScroll() const { return m_Scroll; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_Scroll << ")";
			return ss.str();
		}

	protected:
		float m_Scroll = 0.0f;
	};
}
