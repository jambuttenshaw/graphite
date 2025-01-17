
group "Dependencies"
    include "vendor/D3D12MemoryAllocator.lua"
group ""

group "Core"

    project "graphiteD3D12"
            kind "SharedLib"
            language "C++"
            cppdialect "C++20"

            files {
                "src/**.h",
                "src/**.cpp"
            }

            includedirs {
                "../../src",

                "src",

                "../../vendor/spdlog/include",
                "../../vendor/imgui/",
                "../../vendor/glm/",

                -- DirectX Libraries
                "vendor/DirectX-Headers/include",
                "vendor/D3D12MemoryAllocator/include",
                "vendor/dxc/lib/"
            }

            links {
                "spdlog",
                "imgui",
                "D3D12MemoryAllocator",
                "graphite",

                "d3d12",
                "dxgi",
                "dxguid",
                "dxcompiler"
            }

            pchheader "graphite_d3d12_pch.h"
            pchsource "src/graphite_d3d12_pch.cpp"

            defines { "GRAPHITE_BUILD" }

            buildoptions { "/utf-8" }

            targetdir ("bin/" .. outputdir .. "/%{prj.name}")
            objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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