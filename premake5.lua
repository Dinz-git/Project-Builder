workspace "Builder"
    architecture "x64"
    configurations {"Debug", "Release", "Dist"}
    startproject "prj"

    filter {"toolset:msc*"}
        buildoptions {
            "/EHsc",
            "/Zc:preprocessor",
            "/Zc:__cplusplus"
        }

    filter {"toolset:gcc or toolset:clang"}
        buildoptions {
            "-pthread",
            "-fexceptions"
        }

    filter {}

include "Builder/prj/premake5.lua"