#pragma once
#include "Graphite/Core/Core.h"
#include "Graphite/Events/Event.h"


namespace Graphite
{

	struct GraphiteWindowDesc
	{
		uint32_t Width;
		uint32_t Height;

		// Unique identifier for this window
		std::wstring WindowName;
		// String to be displayed at the top of the window
		std::wstring WindowTitle;
	};


	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(const GraphiteWindowDesc& windowDesc);
		~Window();

		DELETE_COPY(Window);
		DELETE_MOVE(Window);

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		void SetEventCallback(const EventCallbackFn& eventCallback);

		// Returns false if window has closed
		bool OnUpdate() const;

	private:
		// static event callback function - specific window specified through hWnd param
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		// Window handle
		HWND m_HWND = nullptr;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		RECT m_WindowRect;

		// Window data required by callbacks
		struct WindowData
		{
			EventCallbackFn EventCallbackFn;
		};
		WindowData m_WindowData;
	};

}
