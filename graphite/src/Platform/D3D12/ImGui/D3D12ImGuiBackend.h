#pragma once


#include "Graphite/ImGui/ImGuiBackend.h"


namespace Graphite::D3D12
{
	extern ImGuiBackend* CreateD3D12ImGuiBackend(const GraphicsContext& graphicsContext, const class DescriptorAllocation& imGuiResources);


	class D3D12ImGuiBackend : public ImGuiBackend
	{
	public:
		D3D12ImGuiBackend(const GraphicsContext& graphicsContext, const class DescriptorAllocation& imGuiResources);
		virtual ~D3D12ImGuiBackend();

		DELETE_COPY(D3D12ImGuiBackend);
		DEFAULT_MOVE(D3D12ImGuiBackend);

		void NewFrame() override;
		void Render(ImDrawData* drawData, CommandRecordingContext* recordingContext) override;
	};

}
