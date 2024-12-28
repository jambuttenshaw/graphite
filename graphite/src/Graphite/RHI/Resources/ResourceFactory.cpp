#include "graphite_pch.h"
#include "ResourceFactory.h"

#include "Graphite/RHI/RHIExceptions.h"
#include "Graphite/Core/Assert.h"

#include "Platform/D3D12/Resources/D3D12ResourceFactory.h"


namespace Graphite
{

	std::unique_ptr<ResourceFactory> ResourceFactory::s_ResourceFactory;

	void ResourceFactory::CreateResourceFactory(const GraphicsContext& graphicsContext)
	{
		s_ResourceFactory = std::make_unique<D3D12::D3D12ResourceFactory>(graphicsContext);
	}

	void ResourceFactory::DestroyResourceFactory()
	{
		s_ResourceFactory.reset();
	}


	ResourceFactory& ResourceFactory::Get()
	{
		GRAPHITE_ASSERT(s_ResourceFactory, "Resource factory has not been created!");
		return *s_ResourceFactory.get();
	}


	// --- Helper Functions ---

	uint64_t ResourceFactory::AlignSize(uint64_t size, uint64_t alignment)
	{
		return (size + (alignment - 1)) & ~(alignment - 1);
	}

}
