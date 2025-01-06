#include "pch.h"
#include "GameLayer.h"

#include "imgui.h"


struct TriangleConstantBufferType
{
	glm::vec4 Color;
};


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
			Graphite::PipelineResourceBindingFrequency::Static,
			/* Resource Slot = */ 0,
			/* Register Space = */ 0,
			static_cast<Graphite::PipelineResourceShaderVisibility>(Graphite::ShaderVisibility_Vertex | Graphite::ShaderVisibility_Pixel)
			),
		Graphite::PipelineResourceDescription::ConstantBuffer(
			"TriangleOffsetConstantBuffer2",
			Graphite::PipelineResourceBindingFrequency::Dynamic,
			/* Resource Slot = */ 1,
			/* Register Space = */ 0,
			Graphite::ShaderVisibility_Vertex
			),
		Graphite::PipelineResourceDescription::ConstantBuffer(
			"TriangleColorConstantBuffer",
			Graphite::PipelineResourceBindingFrequency::Static,
			/* Resource Slot = */ 1,
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

	m_ConstantBuffer = Graphite::ResourceFactory::Get().CreateConstantBuffer<TriangleConstantBufferType>(1, 1);
	TriangleConstantBufferType cb
	{
		.Color = { 0.0f, 0.0f, 1.0f, 1.0f }
	};
	m_ConstantBuffer->CopyElement(0, 0, &cb, sizeof(cb));

	// Bind static resources
	//m_PipelineStaticResources.SetConstantBufferView(0, *m_ConstantBuffer);
	//m_GraphicsPipeline->BindStaticResources(&m_PipelineStaticResources);
}


void GameLayer::OnUpdate()
{
	ImGui::ShowDemoWindow();
}


void GameLayer::OnRender()
{
	Graphite::GraphicsContext* graphicsContext = Graphite::g_Application->GetGraphicsContext();
	Graphite::Window* window = Graphite::g_Application->GetWindow();

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
		glm::vec4 clearColor{ 1.0f, 0.0f, 0.0f, 1.0f };
		Graphite::CPUDescriptorHandle rtv = graphicsContext->GetBackBufferRenderTargetView();

		recordingContext->ClearRenderTargetView(rtv, clearColor);

		recordingContext->SetRenderTargets(1, rtv, std::nullopt);

		recordingContext->SetGraphicsPipelineState(*m_GraphicsPipeline);

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
