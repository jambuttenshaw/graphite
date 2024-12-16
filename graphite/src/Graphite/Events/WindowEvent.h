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


	class GRAPHITE_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width)
			, m_Height(height)
		{}
		virtual ~WindowResizeEvent() = default;

		DEFINE_EVENT_TYPE(WindowResize, EventCategoryWindow);

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: (" << m_Width << "x" << m_Height << ")";
			return ss.str();
		}

	private:
		uint32_t m_Width = static_cast<uint32_t>(-1);
		uint32_t m_Height = static_cast<uint32_t>(-1);
	};


	class GRAPHITE_API WindowToggleFullscreenEvent : public Event
	{
	public:
		WindowToggleFullscreenEvent() = default;
		virtual ~WindowToggleFullscreenEvent() = default;

		DEFINE_EVENT_TYPE(WindowToggleFullscreen, EventCategoryWindow);

	};

}
