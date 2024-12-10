#pragma once


int main()
{
	Graphite::Log::InitLogging();
	auto app = Graphite::CreateApplication();

	app->Run();

	app.reset();

	return 0;
}
