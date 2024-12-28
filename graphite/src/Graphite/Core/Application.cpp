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
					D3D12_VIEWPORT viewports = { 0.0f, 0.0f, static_cast<float>(m_Window->GetWidth()), static_cast<float>(m_Window->GetHeight()) };
					recordingContext->SetViewports({ &viewports, 1 });
					D3D12_RECT scissorRect = { 0, 0, static_cast<LONG>(m_Window->GetWidth()), static_cast<LONG>(m_Window->GetHeight()) };
					recordingContext->SetScissorRects({ &scissorRect, 1 });

					// Record commands
					auto rtv = m_GraphicsContext->GetBackBufferRenderTargetView();
					float clearColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
					recordingContext->ClearRenderTargetView(rtv, clearColor);

					recordingContext->SetRenderTargets({ &rtv, 1 }, std::nullopt);

					recordingContext->SetPipelineState(m_GraphicsPipeline->GetPipelineState());
					recordingContext->SetGraphicsRootSignature(m_GraphicsPipeline->GetRootSignature());

					recordingContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

					recordingContext->SetIndexBuffer(m_IndexBuffer->GetIndexBufferView());
					const auto& vb = m_VertexBuffer->GetVertexBufferView();
					recordingContext->SetVertexBuffers(0, { &vb, 1 });

					recordingContext->DrawIndexedInstanced(m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);

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
