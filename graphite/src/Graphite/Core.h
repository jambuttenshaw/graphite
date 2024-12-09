#pragma once

#ifdef GRAPHITE_BUILD
	#define GRAPHITE_API __declspec(dllexport)
#else
	#define GRAPHITE_API __declspec(dllimport)
#endif
