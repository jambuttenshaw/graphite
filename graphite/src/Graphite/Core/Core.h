#pragma once

// Graphite exports
// All symbols declared as GRAPHITE_API will be made available for export from the library

#ifdef GRAPHITE_BUILD
	#define GRAPHITE_API __declspec(dllexport)
#else
	#define GRAPHITE_API __declspec(dllimport)
#endif


#ifdef _DEBUG
	#define GRAPHITE_DEBUG
#else
	#define GRAPHITE_RELEASE
#endif


// Class helpers
#define DEFAULT_COPY(T) T(const T&) = default; \
						T& operator=(const T&) = default;
#define DEFAULT_MOVE(T) T(T&&) = default; \
						T& operator=(T&&) = default;

#define DELETE_COPY(T)  T(T&) = delete; \
					    T& operator=(const T&) = delete;
#define DELETE_MOVE(T)  T(T&&) = delete; \
					    T& operator=(T&&) = delete;

// Exported versions
#define GRAPHITE_API_DEFAULT_COPY(T) GRAPHITE_API T(const T&) = default; \
									 GRAPHITE_API T& operator=(const T&) = default;
#define GRAPHITE_API_DEFAULT_MOVE(T) GRAPHITE_API T(T&&) = default; \
									 GRAPHITE_API T& operator=(T&&) = default;

#define GRAPHITE_API_DELETE_COPY(T)  GRAPHITE_API T(T&) = delete; \
									 GRAPHITE_API T& operator=(const T&) = delete;
#define GRAPHITE_API_DELETE_MOVE(T)  GRAPHITE_API T(T&&) = delete; \
									 GRAPHITE_API T& operator=(T&&) = delete;

