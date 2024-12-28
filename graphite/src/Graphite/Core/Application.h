#pragma once

#include "Core.h"
#include "Graphite/Events/Event.h"
#include "Graphite/Layers/LayerStack.h"


namespace Graphite
{
	class Window;
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

	protected:
		// Interface for clients to implement
		GRAPHITE_API virtual void OnInit() {}

	private:

		void OnEvent(Event& event);

	protected:
		std::unique_ptr<GraphicsContext> m_GraphicsContext;

	private:
		bool m_Running = true;

		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;

		std::unique_ptr<UploadBuffer> m_VertexBuffer;
		std::unique_ptr<UploadBuffer> m_IndexBuffer;

		std::unique_ptr<GraphicsPipeline> m_GraphicsPipeline;
	};

	extern std::unique_ptr<Application> CreateApplication();

}
