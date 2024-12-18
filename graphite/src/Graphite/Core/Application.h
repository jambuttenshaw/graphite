#pragma once

#include "Core.h"
#include "Graphite/Events/Event.h"


namespace Graphite
{
	class Window;
	class GraphicsContext;

	class Application
	{
	public:
		GRAPHITE_API Application();
		GRAPHITE_API virtual ~Application();

		GRAPHITE_API_DELETE_COPY(Application);
		GRAPHITE_API_DEFAULT_MOVE(Application);

		GRAPHITE_API int Run();

	protected:
		// Interface for clients to implement
		GRAPHITE_API virtual void OnInit() {}

	private:

		void OnEvent(Event& event);

	protected:
		std::unique_ptr<GraphicsContext> m_GraphicsContext;

	private:
		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
	};

	extern std::unique_ptr<Application> CreateApplication();

}
