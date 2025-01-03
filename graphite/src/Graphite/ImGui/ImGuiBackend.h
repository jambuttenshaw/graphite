#pragma once

#include "Graphite/Core/Core.h"


// Forward declarations
struct ImDrawData;

namespace Graphite
{
	class GraphicsContext;
	class DescriptorAllocation;
	class CommandRecordingContext;
}


namespace Graphite
{
	// Interface to a platform-specific ImGui backend
	class ImGuiBackend
	{
	public:
		static std::unique_ptr<ImGuiBackend> Create(const GraphicsContext& graphicsContext, const DescriptorAllocation& imGuiResources);

	public:
		ImGuiBackend() = default;
		virtual ~ImGuiBackend() = default;

		DELETE_COPY(ImGuiBackend)
		DEFAULT_MOVE(ImGuiBackend)

		virtual void NewFrame() = 0;
		virtual void Render(ImDrawData* drawData, CommandRecordingContext* recordingContext) = 0;
	};

}
