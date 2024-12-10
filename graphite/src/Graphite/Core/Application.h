#pragma once

#include "Core.h"


namespace Graphite
{
	class WindowsWindow;


	class GRAPHITE_API Application
	{
	public:
		Application();
		virtual ~Application();

		int Run();

	protected:
		// Interface for clients to implement
		virtual void OnInit() {}

	private:
		std::unique_ptr<WindowsWindow> m_Window;
	};

	extern std::unique_ptr<Application> CreateApplication();

}
