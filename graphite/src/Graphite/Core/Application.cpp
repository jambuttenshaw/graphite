#include "pch.h"
#include "Application.h"

#include "Log.h"
#include "Graphite/Window/Window.h"

#include "Graphite/Events/WindowEvent.h"

// Graphics
#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/CommandRecordingContext.h"

#include "Graphite/RHI/Resources/Geometry.h"

#include "Graphite/RHI/Resources/ResourceFactory.h"
#include "Graphite/RHI/Pipelines/PipelineFactory.h"


namespace Graphite
{

	Application::Application()
	{
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
			.BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM,

			// Allow for one recording context per CPU core
			.MaxRecordingContextsPerFrame = std::thread::hardware_concurrency()
		};
		m_GraphicsContext = std::make_unique<GraphicsContext>(graphicsContextDesc);


		// Create resources

		// Define geometry
		Vertex_Position vertices[] = {
			{ { -0.5f, -0.5f, 0.0f } },
			{ { 0.0f, 0.5f, 0.0f } },
			{ { 0.5f, -0.5f, 0.0f } },
		};
		uint16_t indices[] = {
			0, 1, 2
		};

		// Create vertex and index buffer
		m_VertexBuffer = ResourceFactory::Get().CreateVertexBuffer(3, Vertex_Position::VertexInputLayout.GetVertexStride(), true);
		m_IndexBuffer = ResourceFactory::Get().CreateIndexBuffer(3, sizeof(indices[0]), true);

		m_VertexBuffer->CopyVertexData(3, vertices);
		m_IndexBuffer->CopyIndexData(3, indices);

		// Create graphics pipeline
		GraphicsPipelineDescription psoDesc
		{
			.InputVertexLayout = &Vertex_Position::VertexInputLayout,
			.VertexShader = {
				.FilePath = L"../graphite/assets/shaders/shaders.hlsl",
				.EntryPoint = L"VSMain"
			},
			.PixelShader = {
				.FilePath = L"../graphite/assets/shaders/shaders.hlsl",
				.EntryPoint = L"PSMain"
			}
		};

		m_GraphicsPipeline = PipelineFactory::Get().CreateGraphicsPipeline(psoDesc);
	}

	Application::~Application()
	{
		m_VertexBuffer.reset();
		m_IndexBuffer.reset();
	}


	int Application::Run()
	{
		while (m_Running)
		{
			// First buffer all messages from all systems capable of submitting events

			// Get all events from window system / OS
			Window::BufferMessages();

			// TODO: Process the buffered events separately


			// Update frame


			// Render frame
			{
				m_GraphicsContext->BeginFrame();

				// Perform all rendering
				{
					CommandRecordingContext* recordingContext = m_GraphicsContext->AcquireRecordingContext();

					// Record commands
					float clearColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
					recordingContext->ClearRenderTargetView(m_GraphicsContext->GetBackBufferRenderTargetView(), clearColor);

					m_GraphicsContext->CloseRecordingContext(recordingContext);
				}

				m_GraphicsContext->EndFrame();
				m_GraphicsContext->Present();
			}
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

		// Handle window resize events
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event)
			{
				this->m_GraphicsContext->ResizeBackBuffer(event.GetWidth(), event.GetHeight());
				return true;
			});
	}

}
