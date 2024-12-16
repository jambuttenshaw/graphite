#include "pch.h"
#include "Application.h"

#include "Log.h"
#include "Graphite/Window/Window.h"

#include "Graphite/Events/WindowEvent.h"


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
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
		
	}


	int Application::Run()
	{
		while (m_Running)
		{
			// First buffer all messages from all systems capable of submitting events

			// Get all events from window system / OS
			Window::BufferMessages();

			// TODO: Process the buffered events separately
		}

		return 0;
	}


	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		// Handle window close events
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent&)
			{
				m_Running = false;
				// Event has been handled
				return true;
			});
	}

}
