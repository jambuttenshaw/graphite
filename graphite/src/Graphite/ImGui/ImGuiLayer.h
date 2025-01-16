#pragma once

#include "Graphite/Layers/Layer.h"
#include "ImGuiBackend.h"

#include "Graphite/Events/KeyboardEvent.h"
#include "Graphite/Events/MouseEvent.h"
#include "Graphite/Events/WindowEvent.h"

#include "Graphite/RHI/Descriptors.h"


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
		ImGuiBackend* m_Backend = nullptr;
		DescriptorAllocation m_ImGuiResources;
	};

}
