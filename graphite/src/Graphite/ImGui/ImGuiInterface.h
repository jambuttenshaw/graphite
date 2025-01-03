#pragma once

#include "Graphite/Core/Core.h"

// Some extra work is required to use the same ImGui context across EXE/DLL boundaries

struct ImGuiContext;
// Taken from imgui.h
typedef void* (*ImGuiMemAllocFunc)(size_t sz, void* user_data);               // Function signature for ImGui::SetAllocatorFunctions()
typedef void  (*ImGuiMemFreeFunc)(void* ptr, void* user_data);                // Function signature for ImGui::SetAllocatorFunctions()


namespace Graphite
{
	GRAPHITE_API extern ImGuiContext* GetImGuiContext();
	GRAPHITE_API extern void GetImGuiAllocators(ImGuiMemAllocFunc* allocFunc, ImGuiMemFreeFunc* freeFunc, void** userData);
}
