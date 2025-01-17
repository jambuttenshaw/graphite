#pragma once

#include "Core.h"

#include "Timer.h"

#include "Graphite/Events/Event.h"
#include "Graphite/Layers/LayerStack.h"

#include "Graphite/Window/Window.h"


namespace Graphite
{
	class Platform;
	class GraphicsContext;


	class Application
	{
	public:
		GRAPHITE_API static Application* Get();
	public:
		GRAPHITE_API Application(Platform& platform);
		GRAPHITE_API virtual ~Application();

		GRAPHITE_API_DELETE_COPY(Application);
		GRAPHITE_API_DEFAULT_MOVE(Application);

		GRAPHITE_API int Run();

		// Manipulate layer stack
		GRAPHITE_API Layer* PushLayer(std::unique_ptr<Layer> layer);
		GRAPHITE_API Layer* PushOverlay(std::unique_ptr<Layer> layer);

		// Get application objects
		GRAPHITE_API Window* GetWindow() const { return m_Window.get(); }
		GRAPHITE_API GraphicsContext* GetGraphicsContext() const { return m_GraphicsContext; }
		GRAPHITE_API Platform* GetPlatform() const { return &m_Platform; }

		// General purpose getters
		GRAPHITE_API float GetDeltaTime() const { return m_Timer.GetDeltaTime(); }

	protected:
		// Interface for clients to implement

		GRAPHITE_API virtual void OnInit()		{}
		// NOTE: All layers will be destroyed before OnDestroy is called
		GRAPHITE_API virtual void OnDestroy()	{}

	private:

		void OnEvent(Event& event);

	protected:
		Platform& m_Platform;

		Timer m_Timer;

		GraphicsContext* m_GraphicsContext;
		std::unique_ptr<Window> m_Window;

	private:
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	// To be implemented by clients to create custom application
	extern std::unique_ptr<Application> CreateApplication();
	
}
