#pragma once

#include "Event.h"
#include "Graphite/Input/KeyCodes.h"

#include "Graphite/Core/Log.h"


namespace Graphite
{

	class GRAPHITE_API KeyEvent : public Event
	{
	public:
		KeyEvent(Key key, char ascii)
			: m_Key(key), m_ASCII(ascii)
		{
#ifdef GRAPHITE_DEBUG
			if (m_Key == Key_None)
			{
				GRAPHITE_LOG_WARN("Key_None was received - Window input handling may be incorrectly configured.");
			}
#endif
		}
		virtual ~KeyEvent() = default;

		inline Key GetKey() const { return m_Key; }

		// ASCII value of key
		// Returns 0 for all Non-ASCII characters
		inline char GetKeyAsASCII() const { return m_ASCII; }

	protected:
		Key m_Key = Key_None;

		// ASCII representation of key
		char m_ASCII = 0;
	};

	class GRAPHITE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(Key key, char ascii)
			: KeyEvent(key, ascii)
		{
		}
		virtual ~KeyPressedEvent() = default;

		DEFINE_EVENT_TYPE(KeyPressed, EventCategoryInput | EventCategoryKeyboard);

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << static_cast<char>(m_ASCII) << " (" << static_cast<uint32_t>(m_Key) << ")";
			return ss.str();
		}
	};


	class GRAPHITE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Key key, char ascii)
			: KeyEvent(key, ascii)
		{
		}
		virtual ~KeyReleasedEvent() = default;

		DEFINE_EVENT_TYPE(KeyReleased, EventCategoryInput | EventCategoryKeyboard);

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << static_cast<char>(m_ASCII) << " (" << static_cast<uint32_t>(m_Key) << ")";
			return ss.str();
		}
	};

}
