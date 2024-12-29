#pragma once

// Graphite master include file

// Required STL (should match pch)
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

// Dependencies
#include <glm/glm.hpp>


// Graphite Core 
#include "Graphite/Core/Core.h"
#include "Graphite/Core/Log.h"

#include "Graphite/Core/Application.h"

// Events
#include "Graphite/Events/KeyboardEvent.h"
#include "Graphite/Events/MouseEvent.h"

#include "Graphite/Input/KeyCodes.h"

// Window
#include "Graphite/Window/Window.h"

// RHI
#include "Graphite/RHI/GraphicsContext.h"
#include "Graphite/RHI/CommandRecordingContext.h"

#include "Graphite/RHI/Resources/ResourceFactory.h"
#include "Graphite/RHI/Resources/ResourceViews.h"

#include "Graphite/RHI/Resources/InputLayout.h"
#include "Graphite/RHI/Resources/Geometry.h"

#include "Graphite/RHI/Pipelines/GraphicsPipeline.h"

/////// Entry Point ///////
#include "Graphite/Core/EntryPoint.h"
