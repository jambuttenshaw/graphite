
group "Dependencies"
    include "vendor/spdlog.lua"
    include "vendor/imgui.lua"
    include "vendor/D3D12MemoryAllocator.lua"
group ""

group "Core"

    project "graphite"
        kind "SharedLib"
        language "C++"
        cppdialect "C++20"

        files {
            "src/Graphite/**.h",
            "src/Graphite/**.cpp"
        }

        includedirs {
            "%{prj.directory}",
            "src",
            "src/Graphite",

            "vendor/spdlog/include",
            "vendor/imgui/",
            "vendor/glm/"
        }

        links {
            "spdlog",
            "imgui"
        }

        pchheader "graphite_pch.h"
        pchsource "src/Graphite/graphite_pch.cpp"

        defines { "GRAPHITE_BUILD" }

        buildoptions { "/utf-8" }

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

        filter "configurations:Debug"
            defines { "GRAPHITE_DEBUG" }
            runtime "Debug"
            symbols "On"

        filter "configurations:Release"
            defines { "GRAPHITE_RELEASE" }
            runtime "Release"
            optimize "On"
            symbols "Off"

    include "premake5_d3d12.lua"
    

group ""
