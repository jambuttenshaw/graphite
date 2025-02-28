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
	// Create vertex and index buffer
	Graphite::InputLayout VertexBufferLayout{
		{ Graphite::VertexAttribute::Position, Graphite::GraphiteFormat_R32G32B32_FLOAT },
		{ Graphite::VertexAttribute::Normal, Graphite::GraphiteFormat_R32G32B32_FLOAT }
	};
	m_VertexBuffer = Graphite::ResourceFactory::Get().CreateVertexBuffer(std::size(positions), VertexBufferLayout);
	m_VertexBuffer->CopyAttribute(Graphite::VertexAttribute::Position, std::span<const glm::vec3>(positions));
	m_VertexBuffer->CopyAttribute(Graphite::VertexAttribute::Normal, std::span<const glm::vec3>(normals));

	m_IndexBuffer = Graphite::ResourceFactory::Get().CreateUploadBuffer<uint16_t>(std::size(indices), 1, 0);
	m_IndexBuffer->CopyElements(0, std::size(indices), 0, indices, sizeof(indices));

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
		.InputVertexLayout = &VertexBufferLayout,
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

	Graphite::GraphicsContext* graphicsContext = Graphite::Application::Get()->GetGraphicsContext();
	m_GraphicsPipeline = graphicsContext->CreateGraphicsPipeline(psoDesc);

	m_PassCB = Graphite::ConstantBuffer<PassConstantBufferType>(1);
	// Create view and projection matrices

	auto window = Graphite::Application::Get()->GetWindow();
	float width = static_cast<float>(window->GetWidth());
	float height = static_cast<float>(window->GetHeight());

	auto viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)));
	auto projMatrix = glm::perspectiveFovLH_ZO(glm::pi<float>() * 0.25f, width, height, 0.1f, 100.0f);
	auto viewProj = projMatrix * viewMatrix;

	m_PassCB.SetElement(0, PassConstantBufferType{ viewProj });

	m_InstanceDataCB = Graphite::ConstantBuffer<InstanceDataConstantBufferType>(1);
	// Create object transforms
	m_InstanceDataCB.SetElement(0, { glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f) )});

	m_DynamicResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Dynamic);

	m_StaticResourceList = Graphite::ResourceViewList::Create(*m_GraphicsPipeline, Graphite::PipelineResourceBindingFrequency::Static);
	m_StaticResourceList.SetConstantBufferView("PassConstantBuffer", *m_PassCB.GetBuffer(), 0);
}


void GameLayer::OnUpdate()
{
	float dt = Graphite::Application::Get()->GetDeltaTime();
	m_Yaw += glm::radians(15.0f) * dt;

	m_InstanceDataCB.SetElement(0, { glm::rotate(glm::mat4(1.0f), m_Yaw, glm::vec3(0.0f, 1.0f, 0.0f)) });

	ImGui::Begin("Debug");

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

		recordingContext->ClearRenderTargetView(rtv, clearColor);

		recordingContext->SetRenderTargets(1, rtv, std::nullopt);

		recordingContext->SetGraphicsPipelineState(*m_GraphicsPipeline);
		recordingContext->SetGraphicsPipelineResources(m_StaticResourceList);

		// The following could be encapsulated as geometry properties of a mesh:
		recordingContext->SetPrimitiveTopology(Graphite::GraphiteTopology_TRIANGLELIST);

		recordingContext->SetVertexBuffers(0, *m_VertexBuffer);

		Graphite::IndexBufferView ibv = Graphite::IndexBufferView::Create(*m_IndexBuffer);
		recordingContext->SetIndexBuffer(ibv);

		// Using dynamic resource lists like this is only possible with inline pipeline resources
		// TODO: Implement transient resource lists that can linearly allocate descriptor tables on the fly
		{
			m_DynamicResourceList.SetConstantBufferView("InstanceDataConstantBuffer", *m_InstanceDataCB.GetBuffer(), 0);
			recordingContext->SetGraphicsPipelineResources(m_DynamicResourceList);

			recordingContext->DrawIndexedInstanced(m_IndexBuffer->GetElementCount(), 1, 0, 0, 0);
		}

		graphicsContext->CloseRecordingContext(recordingContext);
	}
	graphicsContext->EndPass();
}
