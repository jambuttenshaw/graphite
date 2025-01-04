#include "graphite_pch.h"
#include "Application.h"

#include "Log.h"
#include "Assert.h"

#include "Graphite/Window/Window.h"
#include "Graphite/Events/WindowEvent.h"

// Graphics
#include "Graphite/ImGui/ImGuiLayer.h"
#include "Graphite/RHI/GraphicsContext.h"


namespace Graphite
{
	Application* g_Application = nullptr;

	Application::Application()
	{
		// Logging must be initialised immediately
		// so that assertions are available
		Log::InitLogging();

		GRAPHITE_ASSERT(!g_Application, "Cannot create a second application!");
		g_Application = this;

		constexpr uint32_t defaultWidth = 1600;
		constexpr uint32_t defaultHeight = 900;

		// Describe the window we want to create
		GraphiteWindowDesc windowDesc = {
			.Width = defaultWidth,
			.Height = defaultHeight,
			.WindowName = L"GraphiteMainWindow",
			.WindowTitle = L"Graphite Engine"
		};

		// Create window
		m_Window = std::make_unique<Window>(windowDesc);
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		// Create graphics context
		GraphiteGraphicsContextDesc graphicsContextDesc
		{
			.WindowHandle = m_Window->GetHandle(),

			.BackBufferWidth = defaultWidth,
			.BackBufferHeight = defaultHeight,
			.BackBufferFormat = GraphiteFormat_R8G8B8A8_UNORM,

			// Allow for one recording context per CPU core
			.MaxRecordingContextsPerFrame = std::thread::hardware_concurrency()
		};
		m_GraphicsContext = GraphicsContext::Create(graphicsContextDesc);
	}

	Application::~Application()
	{
	}


	int Application::Run()
	{
		m_Timer.Reset();

		PushOverlay(std::make_unique<ImGuiLayer>());
		OnInit();

		while (m_Running)
		{
			// First buffer all messages from all systems capable of submitting events

			// Get all events from window system / OS
			Window::BufferMessages();

			// TODO: Process the buffered events separately


			// Update frame
			(void)m_Timer.Tick();

			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate();
			}


			// Render frame
			m_GraphicsContext->BeginFrame();

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}

			m_GraphicsContext->EndFrame();
			m_GraphicsContext->Present();
		}

		// Wait for all GPU work to finish before cleaning up
		m_GraphicsContext->WaitForGPUIdle();

		// Destroy all layers before destroying client application
		m_LayerStack.Clear();
		OnDestroy();

		return 0;
	}

	Layer* Application::PushLayer(std::unique_ptr<Layer> layer)
	{
		return m_LayerStack.PushLayer(std::move(layer));
	}

	Layer* Application::PushOverlay(std::unique_ptr<Layer> layer)
	{
		return m_LayerStack.PushOverlay(std::move(layer));
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

		// Handle window resize events
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event)
			{
				this->m_GraphicsContext->ResizeBackBuffer(event.GetWidth(), event.GetHeight());
				return true;
			});


		// Dispatch events to layers in reverse order - to be received by overlays first
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.IsHandled())
			{
				break;
			}
		}
	}

}
