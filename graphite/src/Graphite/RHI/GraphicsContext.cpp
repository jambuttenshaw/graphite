#include "graphite_pch.h"
#include "GraphicsContext.h"


namespace Graphite
{
	GraphicsContext* g_GraphicsContext = nullptr;

	GraphicsContext* GraphicsContext::Get()
	{
		GRAPHITE_ASSERT(g_GraphicsContext, "Graphics context does not exist!");
		return g_GraphicsContext;
	}

	GraphicsContext::GraphicsContext(const GraphicsContextDesc& contextDesc)
		: m_BackBufferWidth(contextDesc.BackBufferWidth)
		, m_BackBufferHeight(contextDesc.BackBufferHeight)
		, m_BackBufferFormat(contextDesc.BackBufferFormat)
	{
		GRAPHITE_ASSERT(!g_GraphicsContext, "Only one graphics context may exist!");
		g_GraphicsContext = this;
	}
}