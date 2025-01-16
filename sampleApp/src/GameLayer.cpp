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

	// Describe the resource layout of the pipeline
	std::vector resourceLayout
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
			),
		Graphite::PipelineResourceDescription::ConstantBuffer(
			"TestConstantBuffer",
			Graphite::PipelineResourceBindingFrequency::Mutable,
			Graphite::PipelineResourceBindingMethod::Default,
			/* Resource Slot = */ 1,
			/* Register Space = */ 0,
			Graphite::ShaderVisibility_All
			),
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
	m_GraphicsPipeline = graphicsContext->CreateGraphicsPipeline(psoDesc);

	m_OffsetConstantBuffer = Graphite::ConstantBuffer<TriangleOffsetConstantBufferType>{ {
		{{ 0.5f,  0.5f}},
		{{-0.5f, -0.5f}}
	} };

	m_ColorConstantBuffer = Graphite::ConstantBuffer<TriangleColorConstantBufferType>{
		{ { 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	m_DynamicResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Dynamic);

	m_StaticResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Static);
	m_StaticResourceList.SetConstantBufferView("TriangleColorConstantBuffer", *m_ColorConstantBuffer.GetBuffer(), 0);
}


void GameLayer::OnUpdate()
{
	ImGui::Begin("Debug");

	{
		auto offset = m_OffsetConstantBuffer.GetElement(0);
		if (ImGui::DragFloat2("Offset 1", &offset.Offset.x, 0.01f))
		{
			m_OffsetConstantBuffer.SetElement(0, offset);
		}
	}
	{
		auto offset = m_OffsetConstantBuffer.GetElement(1);
		if (ImGui::DragFloat2("Offset 2", &offset.Offset.x, 0.01f))
		{
			m_OffsetConstantBuffer.SetElement(1, offset);

		}
	}
	{
		auto color = m_ColorConstantBuffer.GetElement(0);
		if (ImGui::ColorEdit3("Color", &color.Color.r))
		{
			m_ColorConstantBuffer.SetElement(0, color);
		}
	}

	ImGui::End();
}


void GameLayer::OnRender()
{
	Graphite::GraphicsContext* graphicsContext = Graphite::g_Application->GetGraphicsContext();
	Graphite::Window* window = Graphite::g_Application->GetWindow();

	// Update the data in the constant buffers
	m_OffsetConstantBuffer.CommitDirtyElements(graphicsContext);
	m_ColorConstantBuffer.CommitDirtyElements(graphicsContext);

	// Ensure resource list is up to date
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

		recordingContext->SetPrimitiveTopology(Graphite::GraphiteTopology_TRIANGLELIST);

		Graphite::VertexBufferView vbv = Graphite::VertexBufferView::Create(*m_VertexBuffer);
		Graphite::IndexBufferView ibv = Graphite::IndexBufferView::Create(*m_IndexBuffer);

		recordingContext->SetVertexBuffers(0, { &vbv, 1 });
		recordingContext->SetIndexBuffer(ibv);

		// Using dynamic resource lists like this is only possible with inline pipeline resources
		{
			m_DynamicResourceList.SetConstantBufferView("TriangleOffsetConstantBuffer", *m_OffsetConstantBuffer.GetBuffer(), 0);
			recordingContext->SetGraphicsPipelineResources(m_DynamicResourceList);

			recordingContext->DrawIndexedInstanced(m_IndexBuffer->GetElementCount(), 1, 0, 0, 0);
		}
		{
			m_DynamicResourceList.SetConstantBufferView("TriangleOffsetConstantBuffer", *m_OffsetConstantBuffer.GetBuffer(), 1);
			recordingContext->SetGraphicsPipelineResources(m_DynamicResourceList);

			recordingContext->DrawIndexedInstanced(m_IndexBuffer->GetElementCount(), 1, 0, 0, 0);
		}

		graphicsContext->CloseRecordingContext(recordingContext);
	}
	graphicsContext->EndPass();
}
