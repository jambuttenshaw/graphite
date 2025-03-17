#include "pch.h"
#include "GameLayer.h"

#include "imgui.h"
#include "RHI/Resources/VertexBuffer.h"

// Geometry definition:
static glm::vec3 positions[] = {
	{-1.0f, 1.0f, -1.0f },
	{1.0f, 1.0f, -1.0f },
	{1.0f, -1.0f, -1.0f },
	{-1.0f, -1.0f, -1.0f },
	
	{1.0f, 1.0f, 1.0f },
	{-1.0f, 1.0f, 1.0f },
	{-1.0f, -1.0f, 1.0f },
	{1.0f, -1.0f, 1.0f },
	
	{-1.0f, 1.0f, 1.0f },
	{-1.0f, 1.0f, -1.0f },
	{-1.0f, -1.0f, -1.0f },
	{-1.0f, -1.0f, 1.0f },
	
	{1.0f, 1.0f, -1.0f },
	{1.0f, 1.0f, 1.0f },
	{1.0f, -1.0f, 1.0f },
	{1.0f, -1.0f, -1.0f },
	
	{-1.0f, 1.0f, 1.0f },
	{1.0f, 1.0f, 1.0f },
	{1.0f, 1.0f, -1.0f },
	{-1.0f, 1.0f, -1.0f },
	
	{-1.0f, -1.0f, -1.0f },
	{1.0f, -1.0f, -1.0f },
	{1.0f, -1.0f, 1.0f },
	{-1.0f, -1.0f, 1.0f }
};

static glm::vec3 normals[] = {
	{0.0f, 0.0f, -1.0f },
	{0.0f, 0.0f, -1.0f },
	{0.0f, 0.0f, -1.0f },
	{0.0f, 0.0f, -1.0f },
	
	{0.0f, 0.0f, 1.0f },
	{0.0f, 0.0f, 1.0f },
	{0.0f, 0.0f, 1.0f },
	{0.0f, 0.0f, 1.0f },
	
	{-1.0f, 0.0f, 0.0f },
	{-1.0f, 0.0f, 0.0f },
	{-1.0f, 0.0f, 0.0f },
	{-1.0f, 0.0f, 0.0f },
	
	{1.0f, 0.0f, 0.0f },
	{1.0f, 0.0f, 0.0f },
	{1.0f, 0.0f, 0.0f },
	{1.0f, 0.0f, 0.0f },
	
	{0.0f, 1.0f, 0.0f },
	{0.0f, 1.0f, 0.0f },
	{0.0f, 1.0f, 0.0f },
	{0.0f, 1.0f, 0.0f },
	
	{0.0f, -1.0f, 0.0f },
	{0.0f, -1.0f, 0.0f },
	{0.0f, -1.0f, 0.0f },
	{0.0f, -1.0f, 0.0f },
};

static uint16_t indices[] = {
	// Front
	0, 1, 2,
	0, 2, 3,
	// Back
	4, 5, 6,
	4, 6, 7,
	// Left
	8, 9, 10,
	8, 10, 11,
	// Right
	12, 13, 14,
	12, 14, 15,
	// Top
	16, 17, 18,
	16, 18, 19,
	// Bottom
	20, 21, 22,
	20, 22, 23
};


void GameLayer::OnAttach()
{
	auto window = Graphite::Application::Get()->GetWindow();
	auto graphicsContext = Graphite::Application::Get()->GetGraphicsContext();

	// Load model
	m_Mesh = Graphite::ModelLoader::LoadModel("assets/teapot.obj");

	// Create a texture to use as depth buffer
	Graphite::Texture2DDesc depthBufferDesc{
		.Width = window->GetWidth(),
		.Height = window->GetHeight(),
		.Format = Graphite::GraphiteFormat_D32_FLOAT,
		.ClearValue = Graphite::TextureClearValue{ .Color = glm::vec4(0.0f), .Depth = 1.0f, .Stencil = 0 },
		.AccessFlags = Graphite::ResourceAccess_DepthStencil
	};
	m_DepthBuffer = Graphite::ResourceFactory::Get().CreateTexture2D(depthBufferDesc);

	 m_DSV = graphicsContext->CreateDepthStencilView(m_DepthBuffer.get(), Graphite::GraphiteFormat_D32_FLOAT);

	// Create graphics pipeline

	// Describe the resource layout of the pipeline
	std::vector resourceLayout
	{
		Graphite::PipelineResourceDescription::ConstantBuffer(
			"PassConstantBuffer",
			Graphite::PipelineResourceBindingFrequency::Static,
			Graphite::PipelineResourceBindingMethod::Inline,
			/* Resource Slot = */ 0,
			/* Register Space = */ 0,
			Graphite::ShaderVisibility_Vertex
			),
		Graphite::PipelineResourceDescription::ConstantBuffer(
			"InstanceDataConstantBuffer",
			Graphite::PipelineResourceBindingFrequency::Dynamic,
			Graphite::PipelineResourceBindingMethod::Inline,
			/* Resource Slot = */ 1,
			/* Register Space = */ 0,
			Graphite::ShaderVisibility_Vertex
			)
	};

	Graphite::GraphicsPipelineDescription psoDesc
	{
		.InputVertexLayout = &m_Mesh->GetInputLayout(),
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

	m_GraphicsPipeline = graphicsContext->CreateGraphicsPipeline(psoDesc);

	m_PassCB = Graphite::ConstantBuffer<PassConstantBufferType>(1);
	// Create view and projection matrices

	float width = static_cast<float>(window->GetWidth());
	float height = static_cast<float>(window->GetHeight());

	auto viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)));
	auto projMatrix = glm::perspectiveFovLH_ZO(glm::pi<float>() * 0.25f, width, height, 0.1f, 100.0f);
	auto viewProj = projMatrix * viewMatrix;

	m_PassCB.SetElement(0, PassConstantBufferType{ viewProj });

	m_InstanceDataCB = Graphite::ConstantBuffer<InstanceDataConstantBufferType>(1);
	// Create object transforms
	m_InstanceDataCB.SetElement(0, { m_CubeTransform.GetLocalToWorld() });

	m_DynamicResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Dynamic);

	m_StaticResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Static);
	m_StaticResourceList.SetConstantBufferView("PassConstantBuffer", *m_PassCB.GetBuffer(), 0);
}


void GameLayer::OnUpdate()
{
	float dt = Graphite::Application::Get()->GetDeltaTime();

	m_InstanceDataCB.SetElement(0, { m_CubeTransform.GetLocalToWorld() });

	ImGui::Begin("Debug");

	ImGui::DragFloat3("Translation", &m_CubeTransform.Translation.x, 0.01f);
	ImGui::SliderAngle("Pitch", &m_CubeTransform.Rotation.x);
	ImGui::SliderAngle("Yaw", &m_CubeTransform.Rotation.y);
	ImGui::SliderAngle("Roll", &m_CubeTransform.Rotation.z);
	ImGui::DragFloat3("Scale", &m_CubeTransform.Scale.x, 0.01f);

	ImGui::End();
}


void GameLayer::OnRender()
{
	Graphite::GraphicsContext* graphicsContext = Graphite::Application::Get()->GetGraphicsContext();
	Graphite::Window* window = Graphite::Application::Get()->GetWindow();

	// Update the data in the constant buffers
	m_PassCB.CommitDirtyElements(graphicsContext);
	m_InstanceDataCB.CommitDirtyElements(graphicsContext);

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
		Graphite::CPUDescriptorHandle dsv = m_DSV.GetCPUHandle();

		recordingContext->ClearRenderTargetView(rtv, clearColor);
		recordingContext->ClearDepthStencilView(dsv, 1.0f, 0);

		recordingContext->SetRenderTargets(1, rtv, dsv);

		recordingContext->SetGraphicsPipelineState(*m_GraphicsPipeline);
		recordingContext->SetGraphicsPipelineResources(m_StaticResourceList);

		// The following could be encapsulated as geometry properties of a mesh:
		recordingContext->SetPrimitiveTopology(Graphite::GraphiteTopology_TRIANGLELIST);

		recordingContext->SetVertexBuffers(0, *m_Mesh->GetVertexBuffer());

		Graphite::IndexBufferView ibv = Graphite::IndexBufferView::Create(*m_Mesh->GetIndexBuffer());
		recordingContext->SetIndexBuffer(ibv);

		// Using dynamic resource lists like this is only possible with inline pipeline resources
		// TODO: Implement transient resource lists that can linearly allocate descriptor tables on the fly
		{
			m_DynamicResourceList.SetConstantBufferView("InstanceDataConstantBuffer", *m_InstanceDataCB.GetBuffer(), 0);
			recordingContext->SetGraphicsPipelineResources(m_DynamicResourceList);

			recordingContext->DrawIndexedInstanced(m_Mesh->GetIndexBuffer()->GetElementCount(), 1, 0, 0, 0);
		}

		graphicsContext->CloseRecordingContext(recordingContext);
	}
	graphicsContext->EndPass();
}
