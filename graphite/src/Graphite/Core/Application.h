#pragma once

#include "Core.h"


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
		std::unique_ptr<Window> m_Window;
	};

	extern std::unique_ptr<Application> CreateApplication();

}
