#pragma once
#include "Graphite/Core/Core.h"


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


	class WindowsWindow
	{
	public:
		WindowsWindow(const GraphiteWindowDesc& windowDesc);
		~WindowsWindow();

		DELETE_COPY(WindowsWindow);
		DELETE_MOVE(WindowsWindow);

	private:
		// Window handle
		HWND m_HWND = nullptr;
		RECT m_WindowRect;

		bool m_Fullscreen = false;
	};

}
