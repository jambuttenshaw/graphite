#include "graphite_pch.h"
#include "Application.h"

#include "Log.h"
#include "Graphite/Window/Window.h"

#include "Graphite/Events/WindowEvent.h"

// Graphics
#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/CommandRecordingContext.h"

#include "Graphite/RHI/Resources/Geometry.h"

#include "Graphite/RHI/Resources/ResourceFactory.h"
#include "Graphite/RHI/Pipelines/GraphicsPipeline.h"
#include "Graphite/RHI/Resources/ResourceViews.h"


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
			.BackBufferFormat = GraphiteFormat_R8G8B8A8_UNORM,

			// Allow for one recording context per CPU core
			.MaxRecordingContextsPerFrame = std::thread::hardware_concurrency()
		};
		m_GraphicsContext = GraphicsContext::Create(graphicsContextDesc);

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
		m_VertexBuffer = ResourceFactory::Get().CreateUploadBuffer(3, 1, Vertex_Position::VertexInputLayout.GetVertexStride(), 0);
		m_IndexBuffer = ResourceFactory::Get().CreateUploadBuffer(3, 1, sizeof(indices[0]), 0);

		m_VertexBuffer->CopyElements(0, 3, 0, vertices, sizeof(vertices));
		m_IndexBuffer->CopyElements(0, 3, 0, indices, sizeof(indices));

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

		m_GraphicsPipeline = GraphicsPipeline::Create(*m_GraphicsContext, psoDesc);
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


			// Update frame
			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			// Render frame
			{
				m_GraphicsContext->BeginFrame();

				// Perform all rendering
				{
					CommandRecordingContext* recordingContext = m_GraphicsContext->AcquireRecordingContext();

					// Set viewport
					Viewport viewports = m_Window->GetDefaultViewport();
					recordingContext->SetViewports({ &viewports, 1 });

					Rectangle scissorRect = m_Window->GetDefaultRectangle();
					recordingContext->SetScissorRects({ &scissorRect, 1 });

					// Record commands
					glm::vec4 clearColor{ 1.0f, 0.0f, 0.0f, 1.0f };
					CPUDescriptorHandle rtv = m_GraphicsContext->GetBackBufferRenderTargetView();

					recordingContext->ClearRenderTargetView(rtv, clearColor);

					recordingContext->SetRenderTargets(1, rtv, std::nullopt);

					recordingContext->SetGraphicsPipelineState(*m_GraphicsPipeline);

					recordingContext->SetPrimitiveTopology(GraphiteTopology_TRIANGLELIST);

					VertexBufferView vbv = VertexBufferView::Create(*m_VertexBuffer);
					IndexBufferView ibv = IndexBufferView::Create(*m_IndexBuffer);

					recordingContext->SetVertexBuffers(0, { &vbv, 1});
					recordingContext->SetIndexBuffer(ibv);

					recordingContext->DrawIndexedInstanced(m_IndexBuffer->GetElementCount(), 1, 0, 0, 0);

					m_GraphicsContext->CloseRecordingContext(recordingContext);
				}

				m_GraphicsContext->EndFrame();
				m_GraphicsContext->Present();
			}
		}

		// Wait for all GPU work to finish before cleaning up
		m_GraphicsContext->WaitForGPUIdle();

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
