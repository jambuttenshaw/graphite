#pragma once

#include "Application.h"


int main()
{
	auto app = Graphite::CreateApplication();

	app->Run();

	app.reset();

	return 0;
}
