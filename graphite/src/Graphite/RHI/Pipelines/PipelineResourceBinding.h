#pragma once

#include "Graphite/Core/Core.h"
#include "PipelineResourceLayout.h"
#include "Graphite/RHI/Descriptors.h"


namespace Graphite
{
	class ConstantBuffer;

	struct PipelineResource
	{
		PipelineResourceType Type;
		PipelineResourceBindingFrequency BindingFrequency;
		uint32_t BindingSlot;
		uint32_t RegisterSpace;
		std::vector<size_t> BindPoints;	// A resource shared between shader stages may have more than one descriptor within the resource view list
	};


	class PipelineResourceSet
	{
	public:
		PipelineResourceSet(PipelineResourceBindingFrequency bindingFrequency);
		~PipelineResourceSet() = default;

		DELETE_COPY(PipelineResourceSet);
		DEFAULT_MOVE(PipelineResourceSet);

		void AddResource(const std::string& name, PipelineResource&& resource);

		void SetBaseRootArgumentIndex(uint32_t baseIndex) { m_BaseRootArgumentIndex = baseIndex; }
		void AddRootArgumentOffset(uint32_t offset);

		// Getters

		PipelineResource& GetResource(const std::string& name);
		const PipelineResource& GetResource(const std::string& name) const;

		inline uint32_t GetDescriptorCount() const { return m_DescriptorCount; }

	private:
		PipelineResourceBindingFrequency m_BindingFrequency;

		std::map<std::string, PipelineResource> m_Resources;
		// The total number of descriptors needed to describe all bind points of all resources
		uint32_t m_DescriptorCount = 0;

		uint32_t m_BaseRootArgumentIndex = 0;
		// A resource view list is one contiguous array of descriptors
		// Those descriptors might describe multiple root arguments
		// These arrays describe which offsets into the array of descriptors
		// is the beginning of unique root arguments
		std::vector<uint32_t> m_RootArgumentOffsets;
	};


	class ResourceViewList
	{
	public:
		// Constructs an invalid resource view list
		GRAPHITE_API ResourceViewList() = default;
		// Constructs a resource view list for a specific pipeline resource set
		// A resource view list can only be applied to one pipeline
		GRAPHITE_API ResourceViewList(const PipelineResourceSet& resourceSet);

		~ResourceViewList();

		GRAPHITE_API_DELETE_COPY(ResourceViewList);
		GRAPHITE_API_DELETE_MOVE(ResourceViewList);

		GRAPHITE_API void SetConstantBufferView(const std::string& resourceName, const ConstantBuffer& constantBuffer);

		// Get a handle to the start of the resource view list
		GRAPHITE_API GPUDescriptorHandle GetHandle() const;

	private:
		const PipelineResourceSet* m_ResourceSet = nullptr;

		// One allocation for each frame in flight
		// This is to allow the lifetime of a resource view list to be more than one frame
		// This handles both changing resources in the list dynamically
		// and for buffered resources (such as constant buffers being updated by the CPU)
		std::vector<DescriptorAllocation> m_DescriptorAllocations;
	};

}
