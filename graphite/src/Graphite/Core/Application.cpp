#include "pch.h"
#include "Application.h"

#include "Log.h"
#include "Graphite/Window/WindowsWindow.h"


namespace Graphite
{

	Application::Application()
	{

		// Describe the window we want to create
		GraphiteWindowDesc windowDesc = {
			.Width = 1600,
			.Height = 900,
			.WindowName = L"GraphiteMainWindow",
			.WindowTitle = L"Graphite Engine"
		};

		m_Window = std::make_unique<WindowsWindow>(windowDesc);
	}

	Application::~Application()
	{
		
	}


	int Application::Run()
	{
		// Main sample loop.
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			// Process any messages in the queue.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		// Return this part of the WM_QUIT message to Windows.
		return static_cast<char>(msg.wParam);
	}

}
