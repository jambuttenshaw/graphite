#pragma once

#include "Graphite/Core/Core.h"
#include "PipelineResourceLayout.h"
#include "Graphite/RHI/Descriptors.h"


namespace Graphite
{
	class ConstantBuffer;

	struct PipelineResource
	{
		PipelineResourceDescription Description;

		// For default binding:
		std::vector<size_t> BindPoints;	// A resource shared between shader stages may have more than one descriptor within the resource view list

		// For inline binding:
		size_t InlineResourceIndex;
	};

	class PipelineResourceSet
	{
	public:
		struct RootArgument
		{
			PipelineResourceBindingMethod BindingMethod;
			union
			{
				struct
				{
					uint32_t DescriptorOffset;
				} DefaultBinding;

				struct
				{
					PipelineResourceType Type;
					uint32_t ResourceOffset;
				} InlineBinding;
			};
		};

	public:
		PipelineResourceSet(PipelineResourceBindingFrequency bindingFrequency);
		~PipelineResourceSet() = default;

		DELETE_COPY(PipelineResourceSet);
		DEFAULT_MOVE(PipelineResourceSet);

		// These should only be used in pipeline construction
		void AddResource(const std::string& name, PipelineResource&& resource);

		void SetBaseRootArgumentIndex(uint32_t baseIndex) { m_BaseRootArgumentIndex = baseIndex; }

		void AddDefaultRootArgument(uint32_t offset);
		void AddInlineRootArgument(PipelineResourceType type, uint32_t offset);

		// Getters

		PipelineResource& GetResource(const std::string& name);
		const PipelineResource& GetResource(const std::string& name) const;

		inline uint32_t GetDescriptorCount() const { return m_DescriptorCount; }
		inline uint32_t GetInlineResourceCount() const { return m_InlineResourceCount; }

		inline uint32_t GetBaseRootArgumentIndex() const { return m_BaseRootArgumentIndex; }
		inline std::span<const RootArgument> GetRootArguments() const { return m_RootArguments; }

	private:
		PipelineResourceBindingFrequency m_BindingFrequency;

		std::map<std::string, PipelineResource> m_Resources;
		// The total number of descriptors needed to describe all bind points of all resources
		uint32_t m_DescriptorCount = 0;
		uint32_t m_InlineResourceCount = 0;

		uint32_t m_BaseRootArgumentIndex = 0;
		std::vector<RootArgument> m_RootArguments;
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
		GRAPHITE_API void SetConstantBufferView(const std::string& resourceName, const ConstantBuffer& constantBuffer, uint32_t element);

		// Call every frame prior to setting on a command context
		GRAPHITE_API void CommitResources();

		// Get a handle to the start of the resource view list
		GRAPHITE_API GPUDescriptorHandle GetDescriptorTableHandle(uint32_t offsetInDescriptors) const;
		GRAPHITE_API GPUVirtualAddress GetInlineResourceHandle(uint32_t inlineResourceOffset) const;
		GRAPHITE_API const PipelineResourceSet& GetPipelineResourceSet() const { return *m_ResourceSet; }

	private:
		const PipelineResourceSet* m_ResourceSet = nullptr;

		uint32_t m_DescriptorCount = 0;
		// Staging descriptors are populated on the CPU timeline
		// And then copied to GPU descriptor heaps at the beginning of each frame
		// The staging descriptors are ALSO buffered - but this is because buffered resources (such as constant buffers)
		// might want to use a different descriptor for each frame in flight
		// so we need (Frames in flight * descriptor count) number of descriptors stored in the resource list
		DescriptorAllocation m_StagingDescriptors;
		// This allocation will have a section for each frame in flight
		// This allows resources to be changed dynamically
		DescriptorAllocation m_ResourcesDescriptors;

		uint32_t m_InlineDescriptorCount = 0;
		std::vector<GPUVirtualAddress> m_InlineDescriptors;

		// Set when new resources are set in the view list
		uint32_t m_FramesDirty = 0;
	};

}
