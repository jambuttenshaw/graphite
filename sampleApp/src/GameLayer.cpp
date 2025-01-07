#include "pch.h"
#include "GameLayer.h"

#include "imgui.h"


void GameLayer::OnAttach()
{
	// Setup
	Graphite::Vertex_Position vertices[] = {
		{ { -0.5f, -0.5f, 0.0f } },
		{ { 0.0f, 0.5f, 0.0f } },
		{ { 0.5f, -0.5f, 0.0f } },
	};
	uint16_t indices[] = {
		0, 1, 2
	};

	// Create vertex and index buffer
	m_VertexBuffer = Graphite::ResourceFactory::Get().CreateUploadBuffer<Graphite::Vertex_Position>(3, 1, 0);
	m_IndexBuffer = Graphite::ResourceFactory::Get().CreateUploadBuffer<uint16_t>(3, 1, 0);

	m_VertexBuffer->CopyElements(0, 3, 0, vertices, sizeof(vertices));
	m_IndexBuffer->CopyElements(0, 3, 0, indices, sizeof(indices));

	// Create graphics pipeline

	// Describe the resource layout of the pipeline - it contains a single constant buffer for the pixel shader
	Graphite::PipelineResourceLayout resourceLayout
	{
		Graphite::PipelineResourceDescription::ConstantBuffer(
			"TriangleOffsetConstantBuffer",
			Graphite::PipelineResourceBindingFrequency::Dynamic,
			Graphite::PipelineResourceBindingMethod::Inline,
			/* Resource Slot = */ 0,
			/* Register Space = */ 0,
			Graphite::ShaderVisibility_Vertex
			),
		Graphite::PipelineResourceDescription::ConstantBuffer(
			"TriangleColorConstantBuffer",
			Graphite::PipelineResourceBindingFrequency::Static,
			Graphite::PipelineResourceBindingMethod::Default,
			/* Resource Slot = */ 0,
			/* Register Space = */ 0,
			Graphite::ShaderVisibility_Pixel
			)
	};

	Graphite::GraphicsPipelineDescription psoDesc
	{
		.InputVertexLayout = &Graphite::Vertex_Position::VertexInputLayout,
		.VertexShader = {
			.FilePath = L"../graphite/assets/shaders/shaders.hlsl",
			.EntryPoint = L"VSMain"
		},
		.PixelShader = {
			.FilePath = L"../graphite/assets/shaders/shaders.hlsl",
			.EntryPoint = L"PSMain"
		},
		.ResourceLayout = &resourceLayout
	};

	Graphite::GraphicsContext* graphicsContext = Graphite::g_Application->GetGraphicsContext();
	m_GraphicsPipeline = Graphite::GraphicsPipeline::Create(*graphicsContext, psoDesc);

	m_OffsetConstantBuffer = Graphite::ResourceFactory::Get().CreateConstantBuffer<TriangleOffsetConstantBufferType>(1, 3);
	m_ColorConstantBuffer = Graphite::ResourceFactory::Get().CreateConstantBuffer<TriangleColorConstantBufferType>(1, 3);

	// Bind static resources
	m_DynamicResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Dynamic);
	m_DynamicResourceList.SetConstantBufferView("TriangleOffsetConstantBuffer", *m_OffsetConstantBuffer);

	m_StaticResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Static);
	m_StaticResourceList.SetConstantBufferView("TriangleColorConstantBuffer", *m_ColorConstantBuffer);
}


void GameLayer::OnUpdate()
{
	ImGui::Begin("Debug");

	ImGui::DragFloat2("Offset", &m_OffsetCBData.Offset.x, 0.01f);
	ImGui::ColorEdit3("Color", &m_ColorCBData.Color.r);

	ImGui::End();
}


void GameLayer::OnRender()
{
	Graphite::GraphicsContext* graphicsContext = Graphite::g_Application->GetGraphicsContext();
	Graphite::Window* window = Graphite::g_Application->GetWindow();

	m_OffsetConstantBuffer->CopyElement(0, graphicsContext->GetCurrentBackBuffer(), &m_OffsetCBData, sizeof(m_OffsetCBData));
	m_ColorConstantBuffer->CopyElement(0, graphicsContext->GetCurrentBackBuffer(), &m_ColorCBData, sizeof(m_ColorCBData));

	m_StaticResourceList.CommitResources();
	m_DynamicResourceList.CommitResources();

	// Perform all rendering
	graphicsContext->BeginPass();
	{
		Graphite::CommandRecordingContext* recordingContext = graphicsContext->AcquireRecordingContext();

		// Set viewport
		Graphite::Viewport viewports = window->GetDefaultViewport();
		recordingContext->SetViewports({ &viewports, 1 });

		Graphite::Rectangle scissorRect = window->GetDefaultRectangle();
		recordingContext->SetScissorRects({ &scissorRect, 1 });

		// Record commands
		glm::vec4 clearColor{ 0.17f, 0.2f, 0.23f, 1.0f };
		Graphite::CPUDescriptorHandle rtv = graphicsContext->GetBackBufferRenderTargetView();

		recordingContext->ClearRenderTargetView(rtv, clearColor);

		recordingContext->SetRenderTargets(1, rtv, std::nullopt);

		recordingContext->SetGraphicsPipelineState(*m_GraphicsPipeline);
		recordingContext->SetGraphicsPipelineResources(m_StaticResourceList);
		recordingContext->SetGraphicsPipelineResources(m_DynamicResourceList);

		recordingContext->SetPrimitiveTopology(Graphite::GraphiteTopology_TRIANGLELIST);

		Graphite::VertexBufferView vbv = Graphite::VertexBufferView::Create(*m_VertexBuffer);
		Graphite::IndexBufferView ibv = Graphite::IndexBufferView::Create(*m_IndexBuffer);

		recordingContext->SetVertexBuffers(0, { &vbv, 1 });
		recordingContext->SetIndexBuffer(ibv);

		recordingContext->DrawIndexedInstanced(m_IndexBuffer->GetElementCount(), 1, 0, 0, 0);

		graphicsContext->CloseRecordingContext(recordingContext);
	}
	graphicsContext->EndPass();
}
