#pragma once


int main()
{
	auto app = Graphite::CreateApplication();
	int result = app->Run();
	app.reset();

	return result;
}
