#pragma once

#include "Core.h"
#include "Graphite/Events/Event.h"


namespace Graphite
{
	class Window;


	class Application
	{
	public:
		GRAPHITE_API Application();
		GRAPHITE_API virtual ~Application();

		GRAPHITE_API int Run();

	protected:
		// Interface for clients to implement
		GRAPHITE_API virtual void OnInit() {}

	private:

		void OnEvent(Event& event);

	private:
		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
	};

	extern std::unique_ptr<Application> CreateApplication();

}
