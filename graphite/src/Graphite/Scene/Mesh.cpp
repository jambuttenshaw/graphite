#include "graphite_pch.h"
#include "Mesh.h"

#include "Graphite/RHI/Resources/Buffer.h"
#include "Graphite/RHI/Resources/VertexBuffer.h"
#include "RHI/Resources/ResourceFactory.h"


namespace Graphite
{
	Mesh::Mesh(uint32_t vertexCount, uint32_t indexCount, InputLayout&& inputLayout)
		: m_InputLayout(std::make_unique<InputLayout>(inputLayout))
	{
		m_VertexBuffer = ResourceFactory::Get().CreateVertexBuffer(vertexCount, inputLayout);
		m_IndexBuffer = ResourceFactory::Get().CreateUploadBuffer<uint32_t>(indexCount, 1, 0);
	}

}
