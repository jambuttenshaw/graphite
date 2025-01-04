#pragma once

#include "Graphite/Layers/Layer.h"
#include "ImGuiBackend.h"

#include "Graphite/Events/KeyboardEvent.h"
#include "Graphite/Events/MouseEvent.h"
#include "Graphite/Events/WindowEvent.h"

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
		void NewFrame();

	private:
		std::unique_ptr<ImGuiBackend> m_Backend;
		DescriptorAllocation m_ImGuiResources;
	};

}
