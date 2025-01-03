#include "graphite_pch.h"
#include "ImGuiInterface.h"

#include "imgui.h"
#include "Graphite/Core/Log.h"


namespace Graphite
{
	ImGuiContext* GetImGuiContext()
	{
		auto context = ImGui::GetCurrentContext();
#ifdef GRAPHITE_DEBUG
		if (!context)
		{
			GRAPHITE_LOG_WARN("Graphite ImGui Context has not been created yet. Do not call GetImGuiContext any earlier than Application::OnInit!");
		}
#endif
		return context;
	}


	void GetImGuiAllocators(ImGuiMemAllocFunc* allocFunc, ImGuiMemFreeFunc* freeFunc, void** userData)
	{
#ifdef GRAPHITE_DEBUG
		if (!ImGui::GetCurrentContext())
		{
			GRAPHITE_LOG_WARN("Graphite ImGui Context has not been created yet. Do not call GetImGuiContext any earlier than Application::OnInit!");
		}
#endif
		ImGui::GetAllocatorFunctions(allocFunc, freeFunc, userData);
	}
}
