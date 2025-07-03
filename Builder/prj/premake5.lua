project "prj"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../bin-int/%{cfg.buildcfg}/%{prj.name}")
    debugdir ("../bin/%{cfg.buildcfg}/%{prj.name}")

    files {
        "include/**.hpp",
        "src/**.cpp"
    }

    includedirs {
        "include"
    }

    externalincludedirs {
    }

    libdirs {
    }

    links {
    }

    filter {"system:windows"}
        defines {"WINDOWS_"}
        systemversion "latest"

        filter {"toolset:msc*"}
            links {
            }

        filter {"toolset:gcc or toolset:clang"}
            links {
            }

            buildoptions {
                "-Wall",
                "-Wextra",
                "-pedantic"
            }

        filter {"configurations:Debug"}
            defines {"DEBUG_"}
            runtime "Debug"
            optimize "Off"
            symbols "On"

        filter {"configurations:Release"}
            defines {"RELEASE_"}
            optimize "On"
            symbols "On"

        filter {"configurations:Dist"}
            defines {"DIST_"}
            runtime "Release"
            optimize "On"
            symbols "Off"

    filter {}