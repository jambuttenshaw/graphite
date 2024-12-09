#include <Graphite.h>

class SampleApplication : public Graphite::Application
{
};

std::unique_ptr<Graphite::Application> Graphite::CreateApplication()
{
	return std::move(std::make_unique<SampleApplication>());
}
