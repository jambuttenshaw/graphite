#pragma once

#include "InputLayout.h"
#include "GPUResource.h"

namespace D3D12MA
{
	class Allocation;
}


namespace Graphite
{
	struct Vertex_Position
	{
		glm::vec3 Position;

		// Input layout for this vertex type
		// TODO: build these from shader reflection
		// TODO: Cache these so they can be shared by compatible vertex types
		static const InputLayout VertexInputLayout;
	};

	// Standard vertex types
	struct Vertex_PositionNormalUV
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;

		// Input layout for this vertex type
		// TODO: build these from shader reflection
		// TODO: Cache these so they can be shared by compatible vertex types
		static const InputLayout VertexInputLayout;
	};


	// Dynamic vertex buffers can be updated by the CPU
	// They will live in CPU-writeable upload heaps instead of default heaps
	// This means they will be slower to render
	class VertexBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		VertexBuffer(D3D12MA::Allocation* allocation, uint32_t vertexCount, uint32_t vertexStride, bool dynamic);
	public:
		virtual ~VertexBuffer() = default;

		DELETE_COPY(VertexBuffer);
		DEFAULT_MOVE(VertexBuffer);

		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::VertexBuffer; }

		inline uint32_t GetVertexCount() const { return m_VertexCount; }
		inline uint32_t GetVertexStride() const { return m_VertexStride; }
		inline const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return m_VertexBufferView; }

		void CopyVertexData(uint32_t vertexCount, const void* vertexData) const;

	private:
		uint32_t m_VertexCount;
		uint32_t m_VertexStride;

		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	};


	// Dynamic index buffers can be updated by the CPU
	// They will live in CPU-writeable upload heaps instead of default heaps
	// This means they will be slower to render
	class IndexBuffer : public GPUResource
	{
	protected:
		friend class ResourceFactory;
		IndexBuffer(D3D12MA::Allocation* allocation, uint32_t indexCount, uint32_t indexSizeInBytes, bool dynamic);
	public:
		virtual ~IndexBuffer() = default;

		DELETE_COPY(IndexBuffer);
		DEFAULT_MOVE(IndexBuffer);

		inline virtual GPUResourceType GetResourceType() const override { return GPUResourceType::IndexBuffer; }

		inline uint32_t GetIndexCount() const { return m_IndexCount; }
		inline uint32_t GetIndexSizeInBytes() const { return m_IndexSizeInBytes; }
		inline const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const { return m_IndexBufferView; }

		void CopyIndexData(uint32_t indexCount, const void* indexData) const;

	private:
		static DXGI_FORMAT IndexSizeInBytesToFormat(uint32_t sizeInBytes);

	private:
		uint32_t m_IndexCount;
		uint32_t m_IndexSizeInBytes;

		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	};
}
