#include "graphite_pch.h"
#include "ImGuiLayer.h"

#include "Graphite/Core/Application.h"
#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/GraphicsContext.h"

#include "imgui.h"
#include "ImGuiBackend.h"
#include "Graphite/RHI/CommandRecordingContext.h"


namespace Graphite
{

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup rendering backend
		GraphicsContext* graphicsContext = g_Application->GetGraphicsContext();

		m_ImGuiResources = graphicsContext->GetResourceDescriptorHeap()->Allocate(1);
		GRAPHITE_ASSERT(m_ImGuiResources.IsValid(), "Failed to allocate ImGui resources.");

		m_Backend = ImGuiBackend::Create(*graphicsContext, m_ImGuiResources);

		// To capture all ImGui usage between updates of this layer
		PrepareNewFrame();
	}

	void ImGuiLayer::OnDetach()
	{
		m_Backend.reset();
		m_ImGuiResources.Free();

		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnRender()
	{
		// Render ImGui
		// This will also end the frame
		ImGui::Render();

		// Submit ImGui draw data to be rendered
		GraphicsContext* graphicsContext = g_Application->GetGraphicsContext();
		auto recordingContext = graphicsContext->AcquireRecordingContext();

		auto rtv = graphicsContext->GetBackBufferRenderTargetView();
		recordingContext->SetRenderTargets(1, rtv, std::nullopt);
		m_Backend->Render(ImGui::GetDrawData(), recordingContext);

		graphicsContext->CloseRecordingContext(recordingContext);

		PrepareNewFrame();
	}

	void ImGuiLayer::OnEvent(Event& event)
	{

	}

	void ImGuiLayer::PrepareNewFrame()
	{
		ImGuiIO& io = ImGui::GetIO();

		auto window = g_Application->GetWindow();
		io.DisplaySize = ImVec2(static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()));

		// Begin the new frame
		m_Backend->NewFrame();
		ImGui::NewFrame();
	}

}
