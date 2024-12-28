#pragma once

// STL
#include <memory>
#include <functional>
#include <algorithm>
#include <thread>
#include <utility>
#include <iostream>

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


// Graphics API
// (DirectX Headers must be included BEFORE d3d12/windows!)
#include <directx/d3dx12.h>

#include <d3d12.h>
#include <dxgi1_6.h>

// Windows API
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <Windows.h>

#include <wrl.h>
#include <shellapi.h>


// Maths library
#include <glm/glm.hpp>
// Math library extensions
