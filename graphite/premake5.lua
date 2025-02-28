
group "Dependencies"
    include "vendor/spdlog.lua"
    include "vendor/imgui.lua"
    include "vendor/assimp.lua"
group ""

group "Core"

    project "graphite"
        kind "SharedLib"
        language "C++"
        cppdialect "C++20"

        files {
            "src/**.h",
            "src/**.cpp"
        }

        includedirs {
            "src",
            "src/Graphite",

            "vendor/_config_headers/",
            "vendor/spdlog/include/",
            "vendor/imgui/",
            "vendor/glm/",
            "vendor/assimp/include/"
        }

        links {
            "spdlog",
            "imgui"
        }

        dependson {
            "assimp"
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
group ""

include "Platform/D3D12"
