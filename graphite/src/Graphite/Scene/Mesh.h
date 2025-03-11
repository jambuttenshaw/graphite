#pragma once


namespace Graphite
{
	class VertexBuffer;
	class UploadBuffer;


	class Mesh
	{
	public:
		Mesh();

		inline const VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer.get(); }
		inline VertexBuffer* GetVertexBuffer() { return m_VertexBuffer.get(); }

	private:
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<UploadBuffer> m_IndexBuffer;
	};

}
