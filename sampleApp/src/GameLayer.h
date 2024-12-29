#pragma once


class GameLayer : public Graphite::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnUpdate() override;

protected:
	std::unique_ptr<Graphite::UploadBuffer> m_VertexBuffer;
	std::unique_ptr<Graphite::UploadBuffer> m_IndexBuffer;

	std::unique_ptr<Graphite::GraphicsPipeline> m_GraphicsPipeline;
};
