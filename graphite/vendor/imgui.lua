project "imgui"
	kind "StaticLib"
	language "C++"
    cppdialect "C++20"

	targetdir ("imgui/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("imgui/bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "imgui"
    }

	files {
		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_internal.h",
		"imgui/imgui_tables.cpp",
		"imgui/imgui_widgets.cpp",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",
		"imgui/imgui_demo.cpp",

        -- imgui backends
        "imgui/backends/imgui_impl_dx12.h",
        "imgui/backends/imgui_impl_dx12.cpp"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
