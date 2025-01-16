project "spdlog"
	kind "StaticLib"
	language "C++"
    cppdialect "C++20"

	targetdir ("spdlog/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("spdlog/bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "spdlog/include"
    }

	files {
        "spdlog/src/spdlog.cpp",
        "spdlog/src/stdout_sinks.cpp",
        "spdlog/src/color_sinks.cpp",
        "spdlog/src/file_sinks.cpp",
        "spdlog/src/async.cpp",
        "spdlog/src/cfg.cpp",
        "spdlog/src/bundled_fmtlib_format.cpp"
	}

    defines {
        "SPDLOG_COMPILED_LIB",
        "SPDLOG_WCHAR_TO_UTF8_SUPPORT"
    }

    buildoptions { "/utf-8" }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
