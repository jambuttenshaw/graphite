#pragma once
#include "Graphite/Core/Core.h"
#include "Graphite/Events/Event.h"

// TODO: Move types used by Window into a more general header
#include "Graphite/RHI/RHITypes.h"


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
		struct WindowData;
			
	public:
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(const GraphiteWindowDesc& windowDesc);
		~Window();

		DELETE_COPY(Window);
		DELETE_MOVE(Window);

		// Exported API for client applications to use
		GRAPHITE_API inline uint32_t GetWidth() const { return m_WindowData.Width; }
		GRAPHITE_API inline uint32_t GetHeight() const { return m_WindowData.Height; }

		GRAPHITE_API Viewport GetDefaultViewport() const;
		GRAPHITE_API Rectangle GetDefaultRectangle() const;

		inline HWND GetHandle() const { return m_HWND; }

		void SetEventCallback(const EventCallbackFn& eventCallback);

	public:
		// Process all new messages and submit them to the message buffer
		// This is a window-agnostic process, but it is still pertinent to the window system
		static void BufferMessages();

	private:
		// static event callback function - specific window specified through hWnd param
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		// Helper function that checks for and submits a resize event
		static void WindowResize(WindowData& windowData);

	private:
		// Window handle
		HWND m_HWND = nullptr;

		// Window data required by callbacks
		struct WindowData
		{
			uint32_t Width = 0;
			uint32_t Height = 0;

			// To avoid unnecessary resize events if the size didn't actually change
			bool Resizing = false;
			uint32_t PreviousWidth = 0;
			uint32_t PreviousHeight = 0;

			EventCallbackFn EventCallbackFn;
		};
		WindowData m_WindowData;
	};

}
