project "sampleApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    files { "src/**.h", "src/**.cpp" }

    includedirs
    {
        "../graphite/vendor/spdlog/include",
        "../graphite/vendor/imgui/",
        "../graphite/vendor/glm/",

        "../graphite/src",
        "../graphite/src/Graphite",
        "../graphite/Platform/D3D12/src",

        "src"
    }

    links
    {
        "spdlog",
        "imgui",

        "graphite",
        "graphiteD3D12"
    }

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    buildoptions { "/utf-8" }

    defines { "SPDLOG_COMPILED_LIBRARY" }

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    postbuildcommands ("{COPYDIR} \"../graphite/bin/" .. outputdir .. "/graphite/\" \"%{cfg.buildtarget.directory}\"\n")
    postbuildcommands ("{COPYDIR} \"../graphite/Platform/D3D12/bin/" .. outputdir .. "/graphiteD3D12/\" \"%{cfg.buildtarget.directory}\"\n")
    postbuildcommands ("{COPYDIR} \"../graphite/Platform/D3D12/vendor/dxc/bin/x64/\" \"%{cfg.buildtarget.directory}\"")
    postbuildcommands ("{COPYDIR} \"../graphite/vendor/assimp/bin/" .. outputdir .. "/assimp/\" \"%{cfg.buildtarget.directory}\"\n")
    

    filter "configurations:Debug"
        defines { "GRAPHITE_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"

        defines { "GRAPHITE_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"
