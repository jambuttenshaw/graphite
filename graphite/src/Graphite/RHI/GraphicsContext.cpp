#include "graphite_pch.h"
#include "GraphicsContext.h"

#include "Graphite/Core/Assert.h"
#include "Platform/D3D12/GraphiteD3D12.h"


namespace Graphite
{
	GraphicsContext* g_GraphicsContext = nullptr;

	std::unique_ptr<GraphicsContext> GraphicsContext::Create(const GraphiteGraphicsContextDesc& contextDesc)
	{
		GRAPHITE_ASSERT(g_GraphicsContext == nullptr, "Cannot create a second graphics context.");
		
		GraphicsContext* graphicsContext = D3D12::CreateD3D12GraphicsContext(contextDesc);
		g_GraphicsContext = graphicsContext;

		return std::unique_ptr<GraphicsContext>(graphicsContext);
	}

}
