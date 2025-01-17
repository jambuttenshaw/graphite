#pragma once


#include "Core/Core.h"
#include "Core/Platform.h"

// Remember: Cannot use any D3D12-specific types in this header!

namespace Graphite::D3D12
{

	class D3D12Platform : public Platform
	{
	public:
		GRAPHITE_API D3D12Platform() = default;
		GRAPHITE_API virtual ~D3D12Platform() = default;

		GRAPHITE_API DELETE_COPY(D3D12Platform);
		GRAPHITE_API DELETE_MOVE(D3D12Platform);

		GRAPHITE_API virtual void InitPlatform(const struct GraphicsContextDesc& graphicsContextDesc) override;
		GRAPHITE_API virtual void SetPlatformImGuiContext(ImGuiContext* imGuiContext,
			void* (*imGuiAllocFunc)(size_t, void*),
			void (*imGuiFreeFunc)(void*, void*)) override;
	};

}
