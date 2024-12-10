#pragma once


int main()
{
	Graphite::Log::InitLogging();
	auto app = Graphite::CreateApplication();

	int result = app->Run();

	app.reset();

	return result;
}
