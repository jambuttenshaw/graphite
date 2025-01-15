
group "Dependencies"
   include "vendor/imgui.lua"
   include "vendor/D3D12MemoryAllocator.lua"
group ""

group "Core"

    project "graphite"
        kind "SharedLib"
        language "C++"
        cppdialect "C++20"
        targetdir "bin/%{cfg.buildcfg}"

        files {
            "src/**.h",
            "src/**.cpp"
        }

        includedirs {
            "src",
            "src/Graphite",

            "vendor/spdlog/include",
            "vendor/imgui/",
            "vendor/glm/",

            -- DirectX Libraries
            "vendor/DirectX-Headers/include",
            "vendor/D3D12MemoryAllocator/include",
            "vendor/dxc/lib/"
        }

        links {
            "D3D12MemoryAllocator",
            "imgui",

            "d3d12",
            "dxgi",
            "dxguid",
            "dxcompiler"
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
