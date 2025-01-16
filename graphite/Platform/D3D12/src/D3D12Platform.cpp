#include "graphite_d3d12_pch.h"
#include "D3D12Platform.h"

#include "D3D12GraphicsContext.h"
#include "Resources/D3D12ResourceFactory.h"
#include "Pipelines/D3D12ShaderCompiler.h"
#include "ImGui/D3D12ImGuiBackend.h"


namespace Graphite::D3D12
{

	void D3D12Platform::InitPlatform(const struct GraphicsContextDesc& graphicsContextDesc)
	{
		m_GraphicsContext = std::make_unique<D3D12GraphicsContext>(graphicsContextDesc);
		m_ResourceFactory = std::make_unique<D3D12ResourceFactory>(*m_GraphicsContext);
		m_ShaderCompiler = std::make_unique<D3D12ShaderCompiler>();
		m_ImGuiBackend = std::make_unique<D3D12ImGuiBackend>();
	}

}
