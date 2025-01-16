#pragma once

// STL
#include <memory>
#include <functional>
#include <algorithm>
#include <thread>
#include <utility>
#include <numeric>
#include <ranges>
#include <iostream>
#include <bitset>
#include <bit>

#include <string>
#include <sstream>
#include <fstream>

#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <array>
#include <queue>
#include <deque>
#include <span>

#include <optional>
#include <mutex>


// Windows API
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <Windows.h>

// Logging library
#ifndef SPDLOG_COMPILED_LIB
#define SPDLOG_COMPILED_LIB
#endif
#ifndef SPDLOG_WCHAR_TO_UTF8_SUPPORT
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#endif
#include <spdlog/spdlog.h>

// Maths library
#include <glm/glm.hpp>
// Math library extensions
