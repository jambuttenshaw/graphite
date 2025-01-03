#include "pch.h"

#include "GameLayer.h"
#include "imgui.h"

class SampleApplication : public Graphite::Application
{
public:

	virtual void OnInit() override
	{
		// Set up ImGui context
		{
			ImGuiMemAllocFunc allocFunc;
			ImGuiMemFreeFunc freeFunc;
			void* userData;
			Graphite::GetImGuiAllocators(&allocFunc, &freeFunc, &userData);

			ImGui::SetCurrentContext(Graphite::GetImGuiContext());
			ImGui::SetAllocatorFunctions(allocFunc, freeFunc);
		}

		(void)PushLayer(std::make_unique<GameLayer>());
	}
};


std::unique_ptr<Graphite::Application> Graphite::CreateApplication()
{
	return std::make_unique<SampleApplication>();
}
