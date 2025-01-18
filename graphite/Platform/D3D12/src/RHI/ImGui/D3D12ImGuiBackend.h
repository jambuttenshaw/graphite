#pragma once


#include "Graphite/ImGui/ImGuiBackend.h"


namespace Graphite::D3D12
{

	class D3D12ImGuiBackend : public ImGuiBackend
	{
	public:
		D3D12ImGuiBackend() = default;
		virtual ~D3D12ImGuiBackend() = default;

		DELETE_COPY(D3D12ImGuiBackend);
		DEFAULT_MOVE(D3D12ImGuiBackend);

		virtual void Init(const GraphicsContext& graphicsContext, const DescriptorAllocation& imGuiResources) override;
		virtual void Destroy() override;

		virtual void NewFrame() override;
		virtual void Render(ImDrawData* drawData, CommandRecordingContext* recordingContext) override;
	};

}
