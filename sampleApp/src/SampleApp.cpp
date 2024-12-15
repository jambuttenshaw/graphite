#include "pch.h"

class SampleApplication : public Graphite::Application
{
public:

	virtual void OnInit() override
	{
		APP_LOG_INFO("Client app is running!");

		Graphite::Application::Run();
	}
};

std::unique_ptr<Graphite::Application> Graphite::CreateApplication()
{
	return std::make_unique<SampleApplication>();
}
