#include "graphite_pch.h"
#include "GraphicsContext.h"


#include "Platform/D3D12/D3D12GraphicsContext.h"


namespace Graphite
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(const GraphiteGraphicsContextDesc& contextDesc)
	{
		return std::unique_ptr<D3D12::D3D12GraphicsContext>(new D3D12::D3D12GraphicsContext(contextDesc));
	}

}
