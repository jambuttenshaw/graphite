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
		GRAPHITE_ASSERT(resource.Description.BindingFrequency == m_BindingFrequency,
			"The binding frequency of this resource does not match the binding frequency of this resource list!");

		if (resource.Description.BindingMethod == PipelineResourceBindingMethod::Default)
		{
			m_DescriptorCount += static_cast<uint32_t>(resource.BindPoints.size());
		}
		else
		{
			m_InlineResourceCount++;
		}

		m_Resources.insert(std::make_pair(name, std::move(resource)));
	}

	void PipelineResourceSet::AddDefaultRootArgument(uint32_t offset)
	{
		m_RootArguments.emplace_back();
		m_RootArguments.back().BindingMethod = PipelineResourceBindingMethod::Default;
		m_RootArguments.back().DefaultBinding.DescriptorOffset = offset;
	}

	void PipelineResourceSet::AddInlineRootArgument(PipelineResourceType type, uint32_t offset)
	{
		m_RootArguments.emplace_back();
		m_RootArguments.back().BindingMethod = PipelineResourceBindingMethod::Inline;
		m_RootArguments.back().InlineBinding.Type = type;
		m_RootArguments.back().InlineBinding.ResourceOffset = offset;
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

		if (m_DescriptorCount > 0)
		{
			m_StagingDescriptors = g_GraphicsContext->AllocateStagingDescriptors(m_DescriptorCount * GraphicsContext::GetBackBufferCount());
			m_ResourcesDescriptors = g_GraphicsContext->AllocateStaticDescriptors(m_DescriptorCount * GraphicsContext::GetBackBufferCount());
		}

		m_InlineDescriptorCount = resourceSet.GetInlineResourceCount();

		// Buffered resources may have a different GPU address for each frame in flight, so allow for 3 addresses to be stored for each inline resource
		m_InlineDescriptors.resize(m_InlineDescriptorCount * GraphicsContext::GetBackBufferCount());
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

	void ResourceViewList::SetConstantBufferView(const std::string& resourceName, const UploadBuffer& buffer, uint32_t element)
	{
		uint32_t instanceCount = buffer.GetInstanceCount();
#ifdef GRAPHITE_DEBUG
		if (!(instanceCount == 1 || instanceCount == GraphicsContext::GetBackBufferCount()))
		{
			GRAPHITE_LOG_WARN("Constant buffer instance count is {}. "
				"It is recommended to use instance count of 1 or the number of frames in flight to correctly synchronise.", instanceCount);
		}
#endif

		// Place resources into staging descriptors that will be copied into the live heap at the correct time
		const PipelineResource& resource = m_ResourceSet->GetResource(resourceName);
		if (resource.Description.BindingMethod == PipelineResourceBindingMethod::Default)
		{
			for (uint32_t i = 0; i < GraphicsContext::GetBackBufferCount(); i++)
			{
				for (const auto& bindPoint : resource.BindPoints)
				{
					g_GraphicsContext->CreateConstantBufferView(
						buffer.GetAddressOfElement(element, std::max(i, instanceCount - 1)),
						buffer.GetElementStride(),
						m_StagingDescriptors.GetCPUHandle(
							(i * m_DescriptorCount) + static_cast<uint32_t>(bindPoint)
						)
					);
				}
			}

			m_FramesDirty = GraphicsContext::GetBackBufferCount();
		}
		else
		{
			for (uint32_t i = 0; i < GraphicsContext::GetBackBufferCount(); i++)
			{
				for (const auto& bindPoint : resource.BindPoints)
				{
					uint32_t index = (i * m_InlineDescriptorCount) + static_cast<uint32_t>(bindPoint);
					m_InlineDescriptors.at(index) = buffer.GetAddressOfElement(element, std::max(i, instanceCount - 1));
				}
			}
		}
	}

	void ResourceViewList::CommitResources()
	{
		if (m_FramesDirty > 0)
		{
			m_FramesDirty--;

			const uint32_t offset = g_GraphicsContext->GetCurrentBackBuffer() * m_DescriptorCount;
			g_GraphicsContext->CopyDescriptors(
				m_StagingDescriptors.GetCPUHandle(offset),
				m_ResourcesDescriptors.GetCPUHandle(offset),
				m_DescriptorCount,
				GraphiteDescriptorHeap_Resource
			);
		}
	}


	GPUDescriptorHandle ResourceViewList::GetDescriptorTableHandle(uint32_t offsetInDescriptors) const
	{
		return m_ResourcesDescriptors.GetGPUHandle((g_GraphicsContext->GetCurrentBackBuffer() * m_DescriptorCount) + offsetInDescriptors);
	}

	GPUVirtualAddress ResourceViewList::GetInlineResourceHandle(uint32_t inlineResourceOffset) const
	{
		return m_InlineDescriptors.at((g_GraphicsContext->GetCurrentBackBuffer() * m_InlineDescriptorCount) + inlineResourceOffset);
	}


	// Factories

	ResourceViewList ResourceViewList::Create(const GraphicsPipeline& pipeline, PipelineResourceBindingFrequency bindingFrequency)
	{
		// This factory needs to exist because GraphicsPipeline::GetPipelineResourceSet needs to be called on a CONST graphics pipeline
		// this factory function has the nice side effect of casting the pipeline to const
		return { pipeline.GetPipelineResourceSet(bindingFrequency) };
	}


}
