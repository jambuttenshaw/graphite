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
		ImGuiBackend() = default;
		virtual ~ImGuiBackend() = default;

		DELETE_COPY(ImGuiBackend);
		DEFAULT_MOVE(ImGuiBackend);

		virtual void Init(const GraphicsContext& graphicsContext, const DescriptorAllocation& imGuiResources) = 0;
		virtual void Destroy() = 0;

		virtual void NewFrame() = 0;
		virtual void Render(ImDrawData* drawData, CommandRecordingContext* recordingContext) = 0;
	};

}
