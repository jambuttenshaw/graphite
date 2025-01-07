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
		m_DescriptorCount = resourceSet.GetDescriptorCount();
		if (m_DescriptorCount == 0)
		{
			// TODO: With inline descriptors, this will no longer be something to warn about
			GRAPHITE_LOG_WARN("Why are you creating a resource view list for a resource set that contains no resources?");
			return;
		}

		m_StagingDescriptors = g_GraphicsContext->AllocateStagingDescriptors(m_DescriptorCount);
		m_ResourcesDescriptors = g_GraphicsContext->AllocateStaticDescriptors(m_DescriptorCount * GraphicsContext::GetBackBufferCount());
	}

	ResourceViewList::~ResourceViewList()
	{
		if (m_StagingDescriptors.IsValid())
		{
			m_StagingDescriptors.Free();
		}
		if (m_ResourcesDescriptors.IsValid())
		{
			m_ResourcesDescriptors.Free();
		}
	}

	void ResourceViewList::SetConstantBufferView(const std::string& resourceName, const ConstantBuffer& constantBuffer)
	{
		// Place resources into staging descriptors that will be copied into the live heap at the correct time
		const PipelineResource& resource = m_ResourceSet->GetResource(resourceName);
		for (const auto& bindPoint : resource.BindPoints)
		{
			g_GraphicsContext->CreateConstantBufferView(
				constantBuffer.GetAddressOfElement(0, 0),
				constantBuffer.GetElementStride(),
				m_StagingDescriptors.GetCPUHandle(static_cast<uint32_t>(bindPoint))
			);
		}

		m_FramesDirty = GraphicsContext::GetBackBufferCount();
	}

	void ResourceViewList::CommitResources()
	{
		if (m_FramesDirty > 0)
		{
			m_FramesDirty--;

			g_GraphicsContext->CopyDescriptors(
				m_StagingDescriptors.GetCPUHandle(),
				m_ResourcesDescriptors.GetCPUHandle(g_GraphicsContext->GetCurrentBackBuffer() * m_DescriptorCount),
				m_DescriptorCount,
				GraphiteDescriptorHeap_RESOURCE
			);
		}
	}


	GPUDescriptorHandle ResourceViewList::GetHandle(uint32_t offsetInDescriptors) const
	{
		return m_ResourcesDescriptors.GetGPUHandle((g_GraphicsContext->GetCurrentBackBuffer() * m_DescriptorCount) + offsetInDescriptors);
	}


	// Factories

	ResourceViewList ResourceViewList::Create(const GraphicsPipeline& pipeline, PipelineResourceBindingFrequency bindingFrequency)
	{
		// This factory needs to exist because GraphicsPipeline::GetPipelineResourceSet needs to be called on a CONST graphics pipeline
		// this factory function has the nice side effect of casting the pipeline to const
		return { pipeline.GetPipelineResourceSet(bindingFrequency) };
	}


}
