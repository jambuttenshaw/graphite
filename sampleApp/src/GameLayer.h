#pragma once


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
	Graphite::ResourceViewList m_PipelineStaticResources;

	std::unique_ptr<Graphite::ConstantBuffer> m_OffsetConstantBuffer;
	std::unique_ptr<Graphite::ConstantBuffer> m_ColorConstantBuffer;
};
