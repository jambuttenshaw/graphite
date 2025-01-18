#include "graphite_d3d12_pch.h"
#include "D3D12ImGuiBackend.h"

#include "backends/imgui_impl_dx12.h"

#include "RHI/D3D12GraphicsContext.h"
#include "RHI/D3D12CommandRecordingContext.h"
#include "RHI/D3D12Types.h"


namespace Graphite::D3D12
{

	void D3D12ImGuiBackend::Init(const GraphicsContext& graphicsContext, const class DescriptorAllocation& imGuiResources)
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

	void D3D12ImGuiBackend::Destroy()
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
