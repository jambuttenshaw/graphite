#include "graphite_pch.h"
#include "D3D12ImGuiBackend.h"

#include "backends/imgui_impl_dx12.h"

#include "Platform/D3D12/D3D12GraphicsContext.h"
#include "Platform/D3D12/D3D12CommandRecordingContext.h"
#include "Platform/D3D12/D3D12Types.h"


namespace Graphite::D3D12
{
	ImGuiBackend* CreateD3D12ImGuiBackend(const GraphicsContext& graphicsContext, const class DescriptorAllocation& imGuiResources)
	{
		return new D3D12ImGuiBackend(graphicsContext, imGuiResources);
	}


	D3D12ImGuiBackend::D3D12ImGuiBackend(const GraphicsContext& graphicsContext, const class DescriptorAllocation& imGuiResources)
	{
		const D3D12GraphicsContext& nativeGraphicsContext = dynamic_cast<const D3D12GraphicsContext&>(graphicsContext);
		D3D12DescriptorHeap* nativeDescriptorHeap = dynamic_cast<D3D12DescriptorHeap*>(imGuiResources.GetHeap());

		ImGui_ImplDX12_Init(
			nativeGraphicsContext.GetDevice(),
			nativeGraphicsContext.GetBackBufferCount(),
			nativeGraphicsContext.GetNativeBackBufferFormat(),
			nativeDescriptorHeap->GetNativeHeap(),
			GraphiteCPUDescriptorToD3D12Descriptor(imGuiResources.GetCPUHandle()),
			GraphiteGPUDescriptorToD3D12Descriptor(imGuiResources.GetGPUHandle())
		);
	}

	D3D12ImGuiBackend::~D3D12ImGuiBackend()
	{
		ImGui_ImplDX12_Shutdown();
	}

	void D3D12ImGuiBackend::NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
	}

	void D3D12ImGuiBackend::Render(ImDrawData* drawData, CommandRecordingContext* recordingContext)
	{
		D3D12CommandRecordingContext* nativeRecordingContext = dynamic_cast<D3D12CommandRecordingContext*>(recordingContext);
		ImGui_ImplDX12_RenderDrawData(drawData, nativeRecordingContext->GetCommandList());
	}
}
