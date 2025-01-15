project "D3D12MemoryAllocator"
	kind "StaticLib"
	language "C++"
    cppdialect "C++20"

	targetdir ("D3D12MemoryAllocator/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("D3D12MemoryAllocator/bin-int/" .. outputdir .. "/%{prj.name}")

    includedirs {
        "D3D12MemoryAllocator/include"
    }

	files {
		"D3D12MemoryAllocator/include/**.h",
        "D3D12MemoryAllocator/src/Shaders/**.h",

        "D3D12MemoryAllocator/src/Common.h",
        "D3D12MemoryAllocator/src/Common.cpp",
        "D3D12MemoryAllocator/src/D3D12MemAlloc.cpp"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
