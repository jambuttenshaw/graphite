#pragma once
#include "GameRenderer.h"


struct TriangleOffsetConstantBufferType
{
	glm::vec2 Offset;
};
struct TriangleColorConstantBufferType
{
	glm::vec4 Color;
};


class GameLayer : public Graphite::Layer
{
public:
	virtual void OnAttach() override;

	virtual void OnUpdate() override;

	virtual std::optional<Graphite::RendererInterface&> GetRenderer() override { return m_Renderer; }

protected:
	GameRenderer m_Renderer;

	std::unique_ptr<Graphite::UploadBuffer> m_VertexBuffer;
	std::unique_ptr<Graphite::UploadBuffer> m_IndexBuffer;

	std::unique_ptr<Graphite::GraphicsPipeline> m_GraphicsPipeline;

	Graphite::ResourceViewList m_DynamicResourceList;
	Graphite::ResourceViewList m_StaticResourceList;

	Graphite::ConstantBuffer<TriangleOffsetConstantBufferType> m_OffsetConstantBuffer;
	Graphite::ConstantBuffer<TriangleColorConstantBufferType> m_ColorConstantBuffer;
};
