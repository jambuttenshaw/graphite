#include "graphite_pch.h"
#include "ResourceFactory.h"

#include "Core/Application.h"
#include "Core/Platform.h"
#include "Graphite/Core/Assert.h"


namespace Graphite
{

	ResourceFactory& ResourceFactory::Get()
	{
		auto resourceFactory = Application::Get()->GetPlatform()->GetResourceFactory();
		GRAPHITE_ASSERT(resourceFactory, "Resource factory has not been created!");
		return *resourceFactory;
	}


	// --- Helper Functions ---

	uint64_t ResourceFactory::AlignSize(uint64_t size, uint64_t alignment)
	{
		return (size + (alignment - 1)) & ~(alignment - 1);
	}

}
