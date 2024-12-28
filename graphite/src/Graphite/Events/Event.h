#pragma once

#include "Graphite/Core/Core.h"


namespace Graphite
{

	enum class GRAPHITE_API EventType
	{
		None = 0,
		// Window events
		WindowClose, WindowResize, WindowToggleFullscreen,
		// Keyboard events
		KeyPressed, KeyReleased,
		// Mouse events
		MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled,
		// User events
		UserEvent
	};

	enum GRAPHITE_API EventCategoryFlags
	{
		EventCategoryNone			= 0,
		EventCategoryWindow			= (1 << 0),
		EventCategoryInput			= (1 << 1),
		EventCategoryKeyboard		= (1 << 2),
		EventCategoryMouse			= (1 << 3),
		EventCategoryMouseButton	= (1 << 4),
		EventCategoryUser			= (1 << 5)
	};


	// Helper macro for filling out event interface
#define DEFINE_EVENT_TYPE(type, categories) \
	static EventType GetStaticType() { return EventType::##type; }\
	inline virtual EventType GetEventType() const override { return GetStaticType(); } \
	inline virtual const char* GetName() const override { return #type; } \
	inline virtual EventCategoryFlags GetCategoryFlags() const override { return static_cast<EventCategoryFlags>(categories); } \


	// Event interface
	class GRAPHITE_API Event
	{
		friend class EventDispatcher;
	public:
		Event() = default;
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual EventCategoryFlags GetCategoryFlags() const = 0;

		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); };

		inline bool IsInCategory(EventCategoryFlags category) const
		{
			return GetCategoryFlags() & category;
		}
		inline bool IsHandled() const { return m_Handled; }

	protected:
		bool m_Handled = false;
	};


	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};


	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
