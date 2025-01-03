#pragma once

#include "Core.h"
#include "Graphite/Events/Event.h"
#include "Graphite/Layers/LayerStack.h"

#include "Graphite/Window/Window.h"


namespace Graphite
{
	class GraphicsContext;

	class UploadBuffer;

	class GraphicsPipeline;

	class Application
	{
	public:
		GRAPHITE_API Application();
		GRAPHITE_API virtual ~Application();

		GRAPHITE_API_DELETE_COPY(Application);
		GRAPHITE_API_DEFAULT_MOVE(Application);

		GRAPHITE_API int Run();

		// Manipulate layer stack
		GRAPHITE_API Layer* PushLayer(std::unique_ptr<Layer> layer);
		GRAPHITE_API Layer* PushOverlay(std::unique_ptr<Layer> layer);

		// Get application objects
		GRAPHITE_API Window* GetWindow() const { return m_Window.get(); }
		GRAPHITE_API GraphicsContext* GetGraphicsContext() const { return m_GraphicsContext.get(); }

	protected:
		// Interface for clients to implement

		GRAPHITE_API virtual void OnInit()		{}
		// NOTE: All layers will be destroyed before OnDestroy is called
		GRAPHITE_API virtual void OnDestroy()	{}

	private:

		void OnEvent(Event& event);

	protected:
		std::unique_ptr<GraphicsContext> m_GraphicsContext;

	private:
		bool m_Running = true;

		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
	};

	GRAPHITE_API extern Application* g_Application;
	extern std::unique_ptr<Application> CreateApplication();
	
}
