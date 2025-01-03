#pragma once

#include "Graphite/Layers/Layer.h"
#include "ImGuiBackend.h"

#include "Graphite/RHI/DescriptorHeap.h"


namespace Graphite
{
	class ImGuiLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;

		void OnRender() override;
		void OnEvent(Event&) override;

	private:
		void PrepareNewFrame();

	private:
		std::unique_ptr<ImGuiBackend> m_Backend;
		DescriptorAllocation m_ImGuiResources;
	};

}
