#pragma once

#include "Graphite/Layers/Layer.h"
#include "ImGuiBackend.h"

#include "Graphite/RHI/Descriptors.h"
#include "Renderer/RendererInterface.h"


namespace Graphite
{
	class ImGuiRenderer : public RendererInterface
	{
	public:

		void Render() override;
	};

	class ImGuiLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event&) override;

	private:
		void NewFrame();

	private:
		ImGuiBackend* m_Backend = nullptr;
		DescriptorAllocation m_ImGuiResources;
	};

}
