#include "pch.h"
#include "Application.h"

#include "Log.h"
#include "Graphite/Window/Window.h"


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

		m_Window = std::make_unique<Window>(windowDesc);
	}

	Application::~Application()
	{
		
	}


	int Application::Run()
	{
		bool running;
		do {
			running = m_Window->OnUpdate();

		} while (running);

		return 0;
	}

}
