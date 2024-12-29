#include "graphite_pch.h"
#include "GraphicsContext.h"


namespace Graphite::D3D12
{
	extern GraphicsContext* CreateD3D12GraphicsContext(const GraphiteGraphicsContextDesc&);
}


namespace Graphite
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(const GraphiteGraphicsContextDesc& contextDesc)
	{
		return std::unique_ptr<GraphicsContext>(D3D12::CreateD3D12GraphicsContext(contextDesc));
	}

}
