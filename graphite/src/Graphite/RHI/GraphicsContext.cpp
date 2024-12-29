#include "graphite_pch.h"
#include "GraphicsContext.h"

#include "Platform/D3D12/GraphiteD3D12.h"


namespace Graphite
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(const GraphiteGraphicsContextDesc& contextDesc)
	{
		return std::unique_ptr<GraphicsContext>(D3D12::CreateD3D12GraphicsContext(contextDesc));
	}

}
