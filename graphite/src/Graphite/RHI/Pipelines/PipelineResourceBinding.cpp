#include "graphite_pch.h"
#include "PipelineResourceBinding.h"

#include "Graphite/Core/Assert.h"

#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/Resources/Buffer.h"


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


	ResourceViewList::ResourceViewList(const PipelineResourceSet& resourceSet)
		: m_ResourceSet(&resourceSet)
	{
		
	}

	ResourceViewList::~ResourceViewList()
	{
		
	}

	void ResourceViewList::SetConstantBufferView(const std::string& resourceName, const ConstantBuffer& constantBuffer)
	{
		
	}

	

}
