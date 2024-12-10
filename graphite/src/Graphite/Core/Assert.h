#pragma once

#include "Log.h"

#ifdef GRAPHITE_DEBUG

#define GRAPHITE_ASSERT(x, msg) if (!(x)) { GRAPHITE_LOG_ERROR("Debug assertion failed in file ({0}) on line ({1}). Message: {2}", __FILE__, __LINE__, msg); DebugBreak(); }

#else

#define GRAPHITE_ASSERT(x, msg)

#endif

