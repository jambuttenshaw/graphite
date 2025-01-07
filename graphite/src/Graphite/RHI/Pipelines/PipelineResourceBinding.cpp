#include "graphite_pch.h"
#include "PipelineResourceBinding.h"

#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/Resources/Buffer.h"
#include "Graphite/RHI/Pipelines/GraphicsPipeline.h"


namespace Graphite
{

	PipelineResourceSet::PipelineResourceSet(PipelineResourceBindingFrequency bindingFrequency)
		: m_BindingFrequency(bindingFrequency)
	{
		
	}

	void PipelineResourceSet::AddResource(const std::string& name, PipelineResource&& resource)
	{
		GRAPHITE_ASSERT(resource.BindingFrequency == m_BindingFrequency,
			"The binding frequency of this resource does not match the binding frequency of this resource list!");
		m_DescriptorCount += static_cast<uint32_t>(resource.BindPoints.size());
		m_Resources.insert(std::make_pair(name, std::move(resource)));
	}

	void PipelineResourceSet::AddRootArgumentOffset(uint32_t offset)
	{
#ifdef GRAPHITE_DEBUG
		if (!m_RootArgumentOffsets.empty())
		{
			GRAPHITE_ASSERT(offset > m_RootArgumentOffsets.back(), "Root argument offsets should be added in order!");
		}
#endif
		m_RootArgumentOffsets.push_back(offset);
	}

	PipelineResource& PipelineResourceSet::GetResource(const std::string& name)
	{
		GRAPHITE_ASSERT(m_Resources.contains(name), "No resource by that name could be located.");
		return m_Resources.at(name);
	}

	const PipelineResource& PipelineResourceSet::GetResource(const std::string& name) const
	{
		GRAPHITE_ASSERT(m_Resources.contains(name), "No resource by that name could be located.");
		return m_Resources.at(name);
	}


	// --- RESOURCE VIEW LIST ---

	ResourceViewList::ResourceViewList(const PipelineResourceSet& resourceSet)
		: m_ResourceSet(&resourceSet)
	{
		m_DescriptorAllocations.resize(GraphicsContext::GetBackBufferCount());
		for (auto& alloc : m_DescriptorAllocations)
		{
			alloc = g_GraphicsContext->AllocateStaticDescriptors(resourceSet.GetDescriptorCount());
		}
	}

	ResourceViewList::~ResourceViewList()
	{
		for (auto& alloc : m_DescriptorAllocations)
		{
			if (alloc.IsValid())
			{
				alloc.Free();
			}
		}
		m_DescriptorAllocations.clear();
	}

	void ResourceViewList::SetConstantBufferView(const std::string& resourceName, const ConstantBuffer& constantBuffer)
	{
		const PipelineResource& resource = m_ResourceSet->GetResource(resourceName);
		for (auto& alloc : m_DescriptorAllocations)
		{
			for (const auto& bindPoint : resource.BindPoints)
			{
				g_GraphicsContext->CreateConstantBufferView(
					constantBuffer.GetAddressOfElement(0, 0),
					constantBuffer.GetElementStride(),
					alloc.GetCPUHandle(static_cast<uint32_t>(bindPoint))
				);
			}
		}
	}

	GPUDescriptorHandle ResourceViewList::GetHandle(uint32_t offsetInDescriptors) const
	{
		return m_DescriptorAllocations.at(g_GraphicsContext->GetCurrentBackBuffer()).GetGPUHandle(offsetInDescriptors);
	}


	// Factories

	ResourceViewList ResourceViewList::Create(const GraphicsPipeline& pipeline, PipelineResourceBindingFrequency bindingFrequency)
	{
		return ResourceViewList(pipeline.GetPipelineResourceSet(bindingFrequency));
	}


}
