#pragma once


namespace Graphite
{
	class VertexBuffer;
	class UploadBuffer;

	class InputLayout;


	class Mesh
	{
	public:
		Mesh(uint32_t vertexCount, uint32_t indexCount, InputLayout&& inputLayout);

		inline const VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer.get(); }
		inline VertexBuffer* GetVertexBuffer() { return m_VertexBuffer.get(); }

		inline const UploadBuffer* GetIndexBuffer() const { return m_IndexBuffer.get(); }
		inline UploadBuffer* GetIndexBuffer() { return m_IndexBuffer.get(); }

		inline const InputLayout& GetInputLayout() const { return *m_InputLayout.get(); }

	private:
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<UploadBuffer> m_IndexBuffer;

		std::unique_ptr<InputLayout> m_InputLayout;
	};

}
