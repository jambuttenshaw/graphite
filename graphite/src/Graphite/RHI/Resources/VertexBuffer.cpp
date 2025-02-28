#include "graphite_pch.h"
#include "VertexBuffer.h"

#include "Buffer.h"
#include "ResourceFactory.h"


namespace Graphite
{

	VertexBuffer::VertexBuffer(uint32_t vertexCount, const InputLayout& inputLayout)
		: GPUResource(
			static_cast<ResourceAccessFlags>(ResourceAccess_CPUWrite | ResourceAccess_GPURead)
		)
		, m_VertexCount(vertexCount)
		, m_InputLayout(&inputLayout)
	{
		
	}

	void VertexBuffer::CreateViews()
	{
		if (m_InputLayout->IsInterleaved())
		{
			// Create one view for the entire buffer
			m_BufferViews.emplace_back(
				GetResourceAddress(),
				m_InputLayout->GetLayoutSizeInBytes() * m_VertexCount,
				m_InputLayout->GetLayoutSizeInBytes()
			);
		}
		else
		{
			uint32_t elementIndex = 0;
			m_BufferViews.resize(m_InputLayout->GetElementCount());

			for (auto& view : m_BufferViews)
			{
				const auto& element = m_InputLayout->GetInputElement(elementIndex);

				const uint64_t offset = static_cast<uint64_t>(m_VertexCount) * element.OffsetInBytes;
				view.BufferAddress = GetResourceAddress() + offset;
				view.BufferSize = static_cast<uint64_t>(m_VertexCount) * element.SizeInBytes;
				view.VertexStride = element.SizeInBytes;

				elementIndex++;
			}
		}
	}


}
