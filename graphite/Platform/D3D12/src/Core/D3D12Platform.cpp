#include "graphite_d3d12_pch.h"
#include "D3D12Platform.h"

#include "RHI/D3D12GraphicsContext.h"
#include "RHI/Resources/D3D12ResourceFactory.h"
#include "RHI/Pipelines/D3D12ShaderCompiler.h"

#include "imgui.h"
#include "RHI/ImGui/D3D12ImGuiBackend.h"


namespace Graphite::D3D12
{

	void D3D12Platform::InitPlatform(const struct GraphicsContextDesc& graphicsContextDesc)
	{
		m_GraphicsContext = std::make_unique<D3D12GraphicsContext>(graphicsContextDesc);
		m_ResourceFactory = std::make_unique<D3D12ResourceFactory>(*m_GraphicsContext);
		m_ShaderCompiler = std::make_unique<D3D12ShaderCompiler>();
		m_ImGuiBackend = std::make_unique<D3D12ImGuiBackend>();
	}

	void D3D12Platform::SetPlatformImGuiContext(ImGuiContext* imGuiContext,
		void* (*imGuiAllocFunc)(size_t, void*),
		void (*imGuiFreeFunc)(void*, void*))
	{
		// This uses the core graphite ImGui context
		ImGui::SetCurrentContext(imGuiContext);
		ImGui::SetAllocatorFunctions(imGuiAllocFunc, imGuiFreeFunc);
	}


}
