#include "graphite_pch.h"
#include "ResourceFactory.h"

#include "Graphite/Core/Assert.h"


namespace Graphite::D3D12
{
	extern ResourceFactory* CreateD3D12ResourceFactory(const GraphicsContext&);
}


namespace Graphite
{

	std::unique_ptr<ResourceFactory> ResourceFactory::s_ResourceFactory;

	void ResourceFactory::CreateResourceFactory(const GraphicsContext& graphicsContext)
	{
		GRAPHITE_ASSERT(!s_ResourceFactory, "Cannot create a second resource factory. Use ResourceFactory::Get() to retrieve the factory.");

		s_ResourceFactory = std::unique_ptr<ResourceFactory>(D3D12::CreateD3D12ResourceFactory(graphicsContext));
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
