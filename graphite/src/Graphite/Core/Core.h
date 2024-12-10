#pragma once

// Graphite exports
// All symbols declared as GRAPHITE_API will be made available for export from the library

#ifdef GRAPHITE_BUILD
	#define GRAPHITE_API __declspec(dllexport)
#else
	#define GRAPHITE_API __declspec(dllimport)
#endif



// Assertions
