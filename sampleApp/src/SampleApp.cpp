#include "pch.h"

#include "D3D12Platform.h"
#include "GameLayer.h"
#include "imgui.h"

class SampleApplication : public Graphite::Application
{
public:
	SampleApplication(Graphite::Platform& platform)
		:Application(platform)
	{}


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


// Use D3D12 platform
Graphite::D3D12::D3D12Platform platform;

std::unique_ptr<Graphite::Application> Graphite::CreateApplication()
{
	return std::make_unique<SampleApplication>(platform);
}
