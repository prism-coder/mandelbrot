include "Dependencies.lua"

-- premake5.lua
workspace "Mandelbrot"
    architecture "x86_64"
    startproject "Mandelbrot"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    flags {
		"MultiProcessorCompile"
	}

    -- Workspace-wide build options for MSVC
    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus", "/utf-8" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Vendor/glfw"
	include "Vendor/glad"
	include "Vendor/imgui"
    include "Vendor/yaml-cpp"
group ""

project "Mandelbrot"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "on"
   
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/**.h",
        "Source/**.cpp",
        "Vendor/stb/stb_image.h",
        "Vendor/stb/stb_image_write.h",
		"Vendor/glm/glm/**.hpp",
		"Vendor/glm/glm/**.inl"
    }

    defines {
        "IMGUI_DEFINE_MATH_OPERATORS",
        "_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
        "YAML_CPP_STATIC_DEFINE",
        "GLM_ENABLE_EXPERIMENTAL"
    }

    includedirs {
        "Source",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.yamlcpp}",
    }

    links {
		"GLFW",
		"Glad",
		"ImGui",
        "yamlcpp",
    }

    postbuildcommands {
        "{COPYDIR} Assets %{cfg.targetdir}/Assets",
        "{COPYDIR} Internal %{cfg.targetdir}/Internal",
        "{COPYFILE} imgui.ini %{cfg.targetdir}/imgui.ini",
        "{COPYFILE} Settings.yaml %{cfg.targetdir}/Settings.yaml"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "MB_PLATFORM_WINDOWS" }
        files { "Internal/Icons/AppIcon/appicon.rc" }

    filter "configurations:Debug"
        defines { "WL_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "MB_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        kind "WindowedApp"
        defines { "MB_DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"