#pragma once


int main()
{
	auto app = Graphite::CreateApplication();

	Graphite::Log::InitLogging();
	app->Run();

	app.reset();

	return 0;
}
