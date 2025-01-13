#pragma once


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
	virtual void OnRender() override;

protected:
	std::unique_ptr<Graphite::UploadBuffer> m_VertexBuffer;
	std::unique_ptr<Graphite::UploadBuffer> m_IndexBuffer;

	std::unique_ptr<Graphite::GraphicsPipeline> m_GraphicsPipeline;

	Graphite::ResourceViewList m_StaticResourceList;
	Graphite::ResourceViewList m_DynamicResourceList;

	Graphite::ConstantBuffer<TriangleOffsetConstantBufferType> m_OffsetConstantBuffer;
	Graphite::ConstantBuffer<TriangleColorConstantBufferType> m_ColorConstantBuffer;
};
