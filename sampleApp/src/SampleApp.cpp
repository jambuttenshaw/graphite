#include "pch.h"

#include "GameLayer.h"


class SampleApplication : public Graphite::Application
{
public:

	virtual void OnInit() override
	{
		(void)PushLayer(std::make_unique<GameLayer>());
	}
};


std::unique_ptr<Graphite::Application> Graphite::CreateApplication()
{
	return std::make_unique<SampleApplication>();
}
