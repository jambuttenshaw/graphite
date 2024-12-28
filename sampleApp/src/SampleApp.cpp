#include "pch.h"


class GameLayer : public Graphite::Layer
{
public:

	virtual void OnAttach() override
	{
		// Setup

	}

	virtual void OnUpdate() override
	{
		
	}
};

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
