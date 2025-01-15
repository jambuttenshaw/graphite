#pragma once


#ifdef GRAPHITE_RELEASE
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif
{
	auto app = Graphite::CreateApplication();
	int result = app->Run();
	app.reset();

	return result;
}
