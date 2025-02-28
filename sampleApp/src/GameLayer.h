#pragma once


struct PassConstantBufferType
{
	glm::mat4 ViewProjectionMatrix;
};
struct InstanceDataConstantBufferType
{
	glm::mat4 WorldMatrix;
};


class GameLayer : public Graphite::Layer
{
public:
	virtual void OnAttach() override;

	virtual void OnUpdate() override;
	virtual void OnRender() override;

protected:
	std::unique_ptr<Graphite::VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Graphite::UploadBuffer> m_IndexBuffer;

	std::unique_ptr<Graphite::GraphicsPipeline> m_GraphicsPipeline;

	Graphite::ResourceViewList m_DynamicResourceList;
	Graphite::ResourceViewList m_StaticResourceList;

	Graphite::ConstantBuffer<PassConstantBufferType> m_PassCB;
	Graphite::ConstantBuffer<InstanceDataConstantBufferType> m_InstanceDataCB;

	float m_Yaw = 0.0f;
};
