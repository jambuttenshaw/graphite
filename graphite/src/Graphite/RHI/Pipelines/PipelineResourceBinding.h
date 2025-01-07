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

		inline uint32_t GetBaseRootArgumentIndex() const { return m_BaseRootArgumentIndex; }
		inline std::span<const uint32_t> GetRootArgumentOffsets() const { return m_RootArgumentOffsets; }

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

		GRAPHITE_API static ResourceViewList Create(const class GraphicsPipeline& pipeline, PipelineResourceBindingFrequency bindingFrequency);

	protected:
		// Constructs a resource view list for a specific pipeline resource set
		// A resource view list can only be applied to one pipeline
		// Use through factories above
		GRAPHITE_API ResourceViewList(const PipelineResourceSet& resourceSet);
	public:
		// Constructs an invalid resource view list
		GRAPHITE_API ResourceViewList() = default;

		GRAPHITE_API ~ResourceViewList();

		GRAPHITE_API_DELETE_COPY(ResourceViewList);
		GRAPHITE_API_DEFAULT_MOVE(ResourceViewList);

		// Update resources within the list
		GRAPHITE_API void SetConstantBufferView(const std::string& resourceName, const ConstantBuffer& constantBuffer);

		// Call every frame prior to setting on a command context
		GRAPHITE_API void CommitResources();

		// Get a handle to the start of the resource view list
		GRAPHITE_API GPUDescriptorHandle GetHandle(uint32_t offsetInDescriptors) const;
		GRAPHITE_API const PipelineResourceSet& GetPipelineResourceSet() const { return *m_ResourceSet; }

	private:
		const PipelineResourceSet* m_ResourceSet = nullptr;

		uint32_t m_DescriptorCount;
		// Staging descriptors are populated on the CPU timeline
		// And then copied to GPU descriptor heaps at the beginning of each frame
		DescriptorAllocation m_StagingDescriptors;
		// This allocation will have a section for each frame in flight
		// This allows resources to be changed dynamically
		DescriptorAllocation m_ResourcesDescriptors;

		// Set when new resources are set in the view list
		uint32_t m_FramesDirty = 0;
	};

}
