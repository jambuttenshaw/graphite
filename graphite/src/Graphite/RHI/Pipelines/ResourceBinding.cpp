#include "graphite_pch.h"
#include "ResourceBinding.h"

#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/Resources/Buffer.h"


namespace Graphite
{
	/*
	ResourceViewList::ResourceViewList(const ResourceTable& table)
	{
		// Get total number of descriptors required by the table
		uint32_t descriptorCount = table.NumConstantBuffers + table.NumShaderResourceViews + table.NumUnorderedAccessViews;

		m_DescriptorAllocations.resize(GraphicsContext::GetBackBufferCount());
		for (auto& allocation : m_DescriptorAllocations)
		{
			allocation = g_GraphicsContext->AllocateStaticDescriptors(descriptorCount);
			GRAPHITE_ASSERT(allocation.IsValid(), "Failed to allocate descriptors");
		}
	}

	void ResourceViewList::SetConstantBufferView(uint32_t index, const ConstantBuffer& constantBuffer)
	{
		// TODO: Not all allocations should be written to at once! This will cause problems
		// Updates should be propagated over subsequent frames
		uint32_t frameIndex = 0;
		for (auto& allocation : m_DescriptorAllocations)
		{
			g_GraphicsContext->CreateConstantBufferView(
				constantBuffer.GetAddressOfElement(0, 0),
				constantBuffer.GetElementStride(),
				allocation.GetCPUHandle(index));
			frameIndex++;
		}
	}

	GPUDescriptorHandle ResourceViewList::GetHandle() const
	{
		GRAPHITE_ASSERT(!m_DescriptorAllocations.empty(), "Invalid resource view list! Did you forget to construct it using the correct resource table?");
		return m_DescriptorAllocations.at(g_GraphicsContext->GetCurrentBackBuffer()).GetGPUHandle();
	}
	*/
}
