#include "graphite_pch.h"
#include "ImGuiBackend.h"

#include "Platform/D3D12/GraphiteD3D12.h"


namespace Graphite
{

	std::unique_ptr<ImGuiBackend> ImGuiBackend::Create(const GraphicsContext& graphicsContext, const DescriptorAllocation& imGuiResources)
	{
		return std::unique_ptr<ImGuiBackend>(D3D12::CreateD3D12ImGuiBackend(graphicsContext, imGuiResources));
	}
}
