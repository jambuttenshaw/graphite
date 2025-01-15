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

        "src"
    }

    links
    {
        "imgui",
        "graphite"
    }

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    buildoptions { "/utf-8" }

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    postbuildcommands ("{COPYDIR} \"../graphite/bin/" .. outputdir .. "/graphite/\" \"%{cfg.buildtarget.directory}\"\n")
    postbuildcommands ("{COPYDIR} \"../graphite/vendor/dxc/bin/x64/\" \"%{cfg.buildtarget.directory}\"")
    

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
