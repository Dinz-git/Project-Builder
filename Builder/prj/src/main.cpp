#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>

static bool CreateFile(const std::filesystem::path& path, const std::string& content) {
    std::ofstream file(path);
    if (!file) {
        std::cerr << "Error creating file: " << path.filename() << std::endl;
        return false;
    }
    file << content;
    file.close();
    return true;
}

static bool CreateFolder(const std::filesystem::path& path) {
    if (!std::filesystem::create_directories(path)) {
        std::cerr << "Error creating folder: " << path.filename() << std::endl;
        return false;
    }
    return true;
}

static bool YesNo(const std::string& question) {
    std::string answer;
    while (true) {
        std::cout << question << " (y/n): ";
        std::getline(std::cin, answer);

        if (answer == "y" || answer == "Y") {
            return true;
        }
        else if (answer == "n" || answer == "N") {
            return false;
        }
    }
}

int main() {
    std::cout << "==============================";

    std::string InMainFolder;
    std::cout << "\nEnter Main folder name: ";
    std::getline(std::cin, InMainFolder);
    std::filesystem::path MainFolder = InMainFolder;

    std::string InProjectFolder;
    std::cout << "\nEnter Project name: ";
    std::getline(std::cin, InProjectFolder);
    std::filesystem::path ProjectFolder = InProjectFolder;

    std::string InExecutableFolder;
    std::cout << "\nEnter Executable name: ";
    std::getline(std::cin, InExecutableFolder);
    std::filesystem::path ExecutableFolder = InExecutableFolder;

    bool Icon = YesNo("\nSupport icon");

    bool Raylib = YesNo("\nLink Raylib");

    bool ENet = YesNo("\nLink ENet");

    bool Engine = YesNo("\nEngine style");

    std::string InEngineFolder;
    std::filesystem::path EngineFolder;
    if (Engine) {
        std::cout << "\nEnter Engine name: ";
        std::getline(std::cin, InEngineFolder);
        EngineFolder = InEngineFolder;
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::string GitIgnore =
        ".vs/\n\n"

        "*.sln\n\n"

        "*.vcxproj\n"
        "*.vcxproj.filters\n"
        "*.vcxproj.user\n\n"

        "makefile\n\n"

        + InProjectFolder + "/bin/\n"
        + InProjectFolder + "/bin-int/\n\n"

        + InProjectFolder + "/Vendor/premake/premake/"
        + (Raylib ? "\n" + InProjectFolder + "/Vendor/raylib/raylib/" : "")
        + (ENet ? "\n" + InProjectFolder + "/Vendor/enet/enet/" : "");

    std::string ProjectPremake =
        "workspace \"" + InProjectFolder + "\"\n"
        "    architecture \"x64\"\n"
        "    configurations {\"Debug\", \"Release\", \"Dist\"}\n"
        "    startproject \"" + InExecutableFolder + "\"\n\n"

        "    filter {\"toolset:msc*\"}\n"
        "        buildoptions {\n"
        "            \"/EHsc\",\n"
        "            \"/Zc:preprocessor\",\n"
        "            \"/Zc:__cplusplus\"\n"
        "        }\n\n"

        "    filter {\"toolset:gcc or toolset:clang\"}\n"
        "        buildoptions {\n"
        "            \"-pthread\",\n"
        "            \"-fexceptions\"\n"
        "        }\n\n"

        "    filter {}\n\n"

        + (Raylib || ENet ? "group \"Vendor\"\n" : "")
        + (Raylib ? "    include \"" + InProjectFolder + "/Vendor/raylib/premake5.lua\"\n" : "")
        + (ENet ? "    include \"" + InProjectFolder + "/Vendor/enet/premake5.lua\"\n" : "")
        + (Raylib || ENet ? "group \"\"\n\n" : "")
        + (Engine ? "group \"Engine\"\n" : "")
        + (Engine ? "    include \"" + InProjectFolder + "/" + InEngineFolder + "/premake5.lua\"\n" : "")
        + (Engine ? "group \"\"\n\n" : "")

        + "include \"" + InProjectFolder + "/" + InExecutableFolder + "/premake5.lua\"";

    std::string Resource =
        "ID ICON \"icon.ico\"";

    std::string EnginePremake =
        "project \"" + InEngineFolder + "\"\n"
        "    kind \"StaticLib\"\n"
        "    language \"C++\"\n"
        "    cppdialect \"C++23\"\n"
        "    staticruntime \"on\"\n\n"

        "    targetdir (\"../bin/%{cfg.buildcfg}/%{prj.name}\")\n"
        "    objdir (\"../bin-int/%{cfg.buildcfg}/%{prj.name}\")\n\n"

        "    pchheader \"pch.hpp\"\n"
        "    pchsource \"src/pch.cpp\"\n\n"

        "    files {\n"
        "        \"include/**.hpp\",\n"
        "        \"src/**.cpp\"\n"
        "    }\n\n"

        "    includedirs {\n"
        "        \"include\"\n"
        "    }\n\n"

        "    externalincludedirs {\n"
        + (Raylib ? "        \"../Vendor/raylib/raylib/src\"" : "")
        + (Raylib && ENet ? ",\n" : (Raylib ? "\n" : ""))
        + (ENet ? "        \"../Vendor/enet/enet/include\"\n" : "")
        + "    }\n\n"

        "    filter {\"system:windows\"}\n"
        "        defines {\"WINDOWS_\"}\n"
        "        systemversion \"latest\"\n\n"

        "        filter {\"toolset:gcc or toolset:clang\"}\n"
        "            buildoptions {\n"
        "                \"-Wall\",\n"
        "                \"-Wextra\",\n"
        "                \"-pedantic\"\n"
        "            }\n\n"

        "        filter {\"configurations:Debug\"}\n"
        "            defines {\"DEBUG_\"}\n"
        "            runtime \"Debug\"\n"
        "            optimize \"Off\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Release\"}\n"
        "            defines {\"RELEASE_\"}\n"
        "            runtime \"Release\"\n"
        "            optimize \"On\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Dist\"}\n"
        "            defines {\"DIST_\"}\n"
        "            runtime \"Release\"\n"
        "            optimize \"On\"\n"
        "            symbols \"Off\"\n\n"

        "    filter {}";

    std::string pch =
        "#include \"pch.hpp\"";

    std::string ExecutablePremake =
        "project \"" + InExecutableFolder + "\"\n"
        "    kind \"ConsoleApp\"\n"
        "    language \"C++\"\n"
        "    cppdialect \"C++23\"\n"
        "    staticruntime \"on\"\n\n"

        "    targetdir (\"../bin/%{cfg.buildcfg}/%{prj.name}\")\n"
        "    objdir (\"../bin-int/%{cfg.buildcfg}/%{prj.name}\")\n"
        "    debugdir (\"../bin/%{cfg.buildcfg}/%{prj.name}\")\n\n"

        + (Engine ? "    pchheader \"pch.hpp\"\n" : "")
        + (Engine ? "    pchsource \"../" + InEngineFolder + "/src/pch.cpp\"\n\n" : "")

        + "    files {\n"
        "        \"include/**.hpp\",\n"
        "        \"src/**.cpp\""
        + (Engine ? ",\n        \"../" + InEngineFolder + "/include/pch.hpp\",\n" : "")
        + (Engine ? "        \"../" + InEngineFolder + "/src/pch.cpp\"" : "")
        + "\n    }\n\n"

        "    includedirs {\n"
        "        \"include\""
        + (Engine ? ",\n        \"../" + InEngineFolder + "/include\"" : "")
        + "\n    }\n\n"

        "    externalincludedirs {\n"
        + (Raylib ? "        \"../Vendor/raylib/raylib/src\"" : "")
        + (Raylib && ENet ? ",\n" : (Raylib ? "\n" : ""))
        + (ENet ? "        \"../Vendor/enet/enet/include\"\n" : "")
        + "    }\n\n"

        "    libdirs {\n"
        + (Raylib ? "        \"../bin/%{cfg.buildcfg}/raylib\"" : "")
        + ((Raylib && ENet) || (Raylib && Engine) ? ",\n" : (Raylib ? "\n" : ""))
        + (ENet ? "        \"../bin/%{cfg.buildcfg}/enet\"" : "")
        + (ENet && Engine ? ",\n" : (ENet ? "\n" : ""))
        + (Engine ? "        \"../bin/%{cfg.buildcfg}/" + InEngineFolder + "\"\n" : "")
        + "    }\n\n"

        "    links {\n"
        + (Raylib ? "        \"raylib\"" : "")
        + ((Raylib && ENet) || (Raylib && Engine) ? ",\n" : (Raylib ? "\n" : ""))
        + (ENet ? "        \"enet\"" : "")
        + (ENet && Engine ? ",\n" : (ENet ? "\n" : ""))
        + (Engine ? "        \"" + InEngineFolder + "\"\n" : "")
        + "    }\n\n"

        "    filter {\"system:windows\"}\n"
        "        defines {\"WINDOWS_\"}\n"
        "        systemversion \"latest\"\n\n"

        "        filter {\"toolset:msc*\"}\n"
        + (Icon ? "            files {\n" : "")
        + (Icon ? "                \"../assets/icon/resource.rc\"\n" : "")
        + (Icon ? "            }\n" : "")
        + "            links {\n"
        + (Raylib ? "                \"raylib.lib\",\n" : "")
        + (ENet ? "                \"enet.lib\",\n" : "")
        + (Engine ? "                \"" + InEngineFolder + ".lib\",\n" : "")
        + (Raylib ? "                \"opengl32.lib\",\n" : "")
        + (Raylib ? "                \"gdi32.lib\",\n" : "")
        + (Raylib || ENet ? "                \"winmm.lib\",\n" : "")
        + (Raylib ? "                \"kernel32.lib\",\n" : "")
        + (Raylib ? "                \"user32.lib\",\n" : "")
        + (Raylib ? "                \"shell32.lib\",\n" : "")
        + (ENet ? "                \"ws2_32.lib\"\n" : "")
        + "            }\n\n"

        "        filter {\"toolset:gcc or toolset:clang\"}\n"
        + (Icon ? "            prebuildcommands {\n" : "")
        + (Icon ? "                \"windres ../assets/icon/resource.rc -O coff -o %{cfg.objdir}/resource.o\"\n" : "")
        + (Icon ? "            }\n" : "")
        + "            links {\n"
        + (Icon ? "                \"%{cfg.objdir}/resource.o\"" : "")
        + (Icon && Raylib ? ",\n" : (Icon ? "\n" : ""))
        + (Raylib ? "                \"opengl32\",\n" : "")
        + (Raylib ? "                \"gdi32\",\n" : "")
        + (Raylib || ENet ? "                \"winmm\",\n" : "")
        + (Raylib ? "                \"kernel32\",\n" : "")
        + (Raylib ? "                \"user32\",\n" : "")
        + (Raylib ? "                \"shell32\",\n" : "")
        + (ENet ? "                \"ws2_32\"\n" : "")
        + "            }\n\n"
        "            buildoptions {\n"
        "                \"-Wall\",\n"
        "                \"-Wextra\",\n"
        "                \"-pedantic\"\n"
        "            }\n\n"
        "        filter {\"configurations:Debug\"}\n"
        "            defines {\"DEBUG_\"}\n"
        "            runtime \"Debug\"\n"
        "            optimize \"Off\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Release\"}\n"
        "            defines {\"RELEASE_\"}\n"
        "            optimize \"On\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Dist\"}\n"
        "            defines {\"DIST_\"}\n"
        "            runtime \"Release\"\n"
        "            optimize \"On\"\n"
        "            symbols \"Off\"\n"
        + (Raylib ? "            kind \"WindowedApp\"\n" : "")
        + (Raylib ? "            entrypoint \"mainCRTStartup\"\n" : "")

        + "\n    filter {}";

    std::string main =
        (Engine ? "#include \"pch.hpp\"\n\n" : "")

        + (std::string)"int main() {\n\n"
        "}";

    std::string ENetLicense =
        "Copyright (c) 2002-2024 Lee Salzman\n\n"

        "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\n"

        "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\n"

        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.";

    std::string ENetPremake =
        "project \"enet\"\n"
        "    kind \"StaticLib\"\n"
        "    language \"C\"\n"
        "    cdialect \"C11\"\n"
        "    staticruntime \"on\"\n\n"

        "    defines {\"_WINSOCK_DEPRECATED_NO_WARNINGS\"}\n\n"

        "    targetdir (\"../../bin/%{cfg.buildcfg}/%{prj.name}\")\n"
        "    objdir (\"../../bin-int/%{cfg.buildcfg}/%{prj.name}\")\n\n"

        "    files {\n"
        "        \"enet/*.c\",\n"
        "        \"enet/include/enet/*.h\"\n"
        "    }\n\n"

        "    includedirs {\n"
        "        \"enet/include\"\n"
        "    }\n\n"

        "    filter {\"system:windows\"}\n"
        "        defines {\"WINDOWS_\"}\n"
        "        systemversion \"latest\"\n\n"

        "        filter {\"toolset:msc*\"}\n"
        "            buildoptions {\"/wd5287\"}\n\n"

        "        filter {\"configurations:Debug\"}\n"
        "            defines {\"DEBUG_\"}\n"
        "            runtime \"Debug\"\n"
        "            optimize \"Off\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Release\"}\n"
        "            defines {\"RELEASE_\"}\n"
        "            runtime \"Release\"\n"
        "            optimize \"On\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Dist\"}\n"
        "            defines {\"DIST_\"}\n"
        "            runtime \"Release\"\n"
        "            optimize \"On\"\n"
        "            symbols \"Off\"\n\n"

        "    filter {}";

    std::string PremakeLicense =
        "Copyright (c) 2003-2022 Jason Perkins and individual contributors.\n"
        "All rights reserved.\n\n"

        "Redistribution and use in source and binary forms, with or without modification,\n"
        "are permitted provided that the following conditions are met:\n\n"

        "  1. Redistributions of source code must retain the above copyright notice,\n"
        "     this list of conditions and the following disclaimer.\n\n"

        "  2. Redistributions in binary form must reproduce the above copyright notice,\n"
        "     this list of conditions and the following disclaimer in the documentation\n"
        "     and/or other materials provided with the distribution.\n\n"

        "  3. Neither the name of Premake nor the names of its contributors may be\n"
        "     used to endorse or promote products derived from this software without\n"
        "     specific prior written permission.\n\n"

        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n"
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n"
        "FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
        "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
        "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n"
        "CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n"
        "OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"
        "OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";

    std::string EXTERNAL_CONFIG_FLAGS =
        "return { \n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Module selection - Some modules could be avoided\n"
        "    -- Mandatory modules: rcore, rlgl, utils\n"
        "    --------------------------------------------------------------------------------------\n"
        "    \"SUPPORT_MODULE_RSHAPES=1\",\n"
        "    \"SUPPORT_MODULE_RTEXTURES=1\",\n"
        "    \"SUPPORT_MODULE_RTEXT=1\",       -- WARNING: It requires SUPPORT_MODULE_RTEXTURES to load sprite font textures\n"
        "    \"SUPPORT_MODULE_RMODELS=1\",\n"
        "    \"SUPPORT_MODULE_RAUDIO=1\",\n\n"

        "    --------------------------------------------------------------------------------------\n"
        "    -- Module: rcore - Configuration Flags\n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Camera module is included (rcamera.h) and multiple predefined cameras are available: free, 1st/3rd person, orbital\n"
        "    \"SUPPORT_CAMERA_SYSTEM=1\",\n"
        "    -- Gestures module is included (rgestures.h) to support gestures detection: tap, hold, swipe, drag\n"
        "    \"SUPPORT_GESTURES_SYSTEM=1\",\n"
        "    -- Include pseudo-random numbers generator (rprand.h), based on Xoshiro128** and SplitMix64\n"
        "    \"SUPPORT_RPRAND_GENERATOR=1\",\n"
        "    -- Mouse gestures are directly mapped like touches and processed by gestures system\n"
        "    \"SUPPORT_MOUSE_GESTURES=1\",\n"
        "    -- Reconfigure standard input to receive key inputs, works with SSH connection.\n"
        "    \"SUPPORT_SSH_KEYBOARD_RPI=1\",\n"
        "    -- Setting a higher resolution can improve the accuracy of time-out intervals in wait functions.\n"
        "    -- However, it can also reduce overall system performance, because the thread scheduler switches tasks more often.\n"
        "    \"SUPPORT_WINMM_HIGHRES_TIMER=1\",\n"
        "    -- Use busy wait loop for timing sync, if not defined, a high-resolution timer is set up and used\n"
        "    --\"SUPPORT_BUSY_WAIT_LOOP=1\",\n"
        "    -- Use a partial-busy wait loop, in this case frame sleeps for most of the time, but then runs a busy loop at the end for accuracy\n"
        "    \"SUPPORT_PARTIALBUSY_WAIT_LOOP=1\",\n"
        "    -- Allow automatic screen capture of current screen pressing F12, defined in KeyCallback()\n"
        "    --\"SUPPORT_SCREEN_CAPTURE=1\",\n"
        "    -- Allow automatic gif recording of current screen pressing CTRL+F12, defined in KeyCallback()\n"
        "    --\"SUPPORT_GIF_RECORDING=1\",\n"
        "    -- Support CompressData() and DecompressData() functions\n"
        "    \"SUPPORT_COMPRESSION_API=1\",\n"
        "    -- Support automatic generated events, loading and recording of those events when required\n"
        "    --\"SUPPORT_AUTOMATION_EVENTS=1\",\n"
        "    -- Support custom frame control, only for advance users\n"
        "    -- By default EndDrawing() does this job: draws everything + SwapScreenBuffer() + manage frame timing + PollInputEvents()\n"
        "    -- Enabling this flag allows manual control of the frame processes, use at your own risk\n"
        "    --\"SUPPORT_CUSTOM_FRAME_CONTROL=1\",\n\n"

        "    -- rcore: Configuration values\n"
        "    --------------------------------------------------------------------------------------\n"
        "    \"MAX_FILEPATH_CAPACITY=8192\",       -- Maximum file paths capacity\n"
        "    \"MAX_FILEPATH_LENGTH=4096\",       -- Maximum length for filepaths (Linux PATH_MAX default value)\n\n"

        "    \"MAX_KEYBOARD_KEYS=512\",       -- Maximum number of keyboard keys supported\n"
        "    \"MAX_MOUSE_BUTTONS=8\",      -- Maximum number of mouse buttons supported\n"
        "    \"MAX_GAMEPADS=4\",      -- Maximum number of gamepads supported\n"
        "    \"MAX_GAMEPAD_AXIS=8\",       -- Maximum number of axis supported (per gamepad)\n"
        "    \"MAX_GAMEPAD_BUTTONS=32\",       -- Maximum number of buttons supported (per gamepad)\n"
        "    \"MAX_GAMEPAD_VIBRATION_TIME=2\",    -- Maximum vibration time in seconds\n"
        "    \"MAX_TOUCH_POINTS=8\",       -- Maximum number of touch points supported\n"
        "    \"MAX_KEY_PRESSED_QUEUE=16 \",      -- Maximum number of keys in the key input queue\n"
        "    \"MAX_CHAR_PRESSED_QUEUE=16\",       -- Maximum number of characters in the char input queue\n\n"

        "    \"MAX_DECOMPRESSION_SIZE=64\",       -- Max size allocated for decompression in MB\n\n"

        "    \"MAX_AUTOMATION_EVENTS=16384\",       -- Maximum number of automation events to record\n\n"

        "    --------------------------------------------------------------------------------------\n"
        "    -- Module: rlgl - Configuration values\n"
        "    --------------------------------------------------------------------------------------\n\n"

        "    -- Enable OpenGL Debug Context (only available on OpenGL 4.3)\n"
        "    --\"RLGL_ENABLE_OPENGL_DEBUG_CONTEXT=1\",\n\n"

        "    -- Show OpenGL extensions and capabilities detailed logs on init\n"
        "    --\"RLGL_SHOW_GL_DETAILS_INFO=1\",\n\n"

        "    \"RL_SUPPORT_MESH_GPU_SKINNING=1\",      -- GPU skinning, comment if your GPU does not support more than 8 VBOs\n\n"

        "    --\"RL_DEFAULT_BATCH_BUFFER_ELEMENTS=4096\",    -- Default internal render batch elements limits\n"
        "    \"RL_DEFAULT_BATCH_BUFFERS=1\",      -- Default number of batch buffers (multi-buffering)\n"
        "    \"RL_DEFAULT_BATCH_DRAWCALLS=256\",      -- Default number of batch draw calls (by state changes: mode, texture)\n"
        "    \"RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS=4\",      -- Maximum number of textures units that can be activated on batch drawing (SetShaderValueTexture())\n\n"

        "    \"RL_MAX_MATRIX_STACK_SIZE=32\",      -- Maximum size of internal Matrix stack\n\n"

        "    \"RL_MAX_SHADER_LOCATIONS=32\",      -- Maximum number of shader locations supported\n\n"

        "    \"RL_CULL_DISTANCE_NEAR=0.01\",      -- Default projection matrix near cull distance\n"
        "    \"RL_CULL_DISTANCE_FAR=1000.0\",      -- Default projection matrix far cull distance\n\n"

        "    -- Default shader vertex attribute locations\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION=0\",\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD=1\",\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL=2\",\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR=3\",\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT=4\",\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2=5\",\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_INDICES=6\",\n"
        "    --#if defined(RL_SUPPORT_MESH_GPU_SKINNING)\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_BONEIDS=7\",\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_LOCATION_BONEWEIGHTS=8\",\n"
        "    --#endif\n\n"

        "    -- Default shader vertex attribute names to set location points\n"
        "    -- NOTE: When a new shader is loaded, the following locations are tried to be set for convenience\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_NAME_POSITION=\\\"vertexPosition\\\"\",    -- Bound by default to shader location: 0\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD=\\\"vertexTexCoord\\\"\",    -- Bound by default to shader location: 1\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_NAME_NORMAL=\\\"vertexNormal\\\"\",      -- Bound by default to shader location: 2\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_NAME_COLOR=\\\"vertexColor\\\"\",       -- Bound by default to shader location: 3\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_NAME_TANGENT=\\\"vertexTangent\\\"\",     -- Bound by default to shader location: 4\n"
        "    \"RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2=\\\"vertexTexCoord2\\\"\",   -- Bound by default to shader location: 5\n\n"

        "    \"RL_DEFAULT_SHADER_UNIFORM_NAME_MVP=\\\"mvp\\\"\",               -- model-view-projection matrix\n"
        "    \"RL_DEFAULT_SHADER_UNIFORM_NAME_VIEW=\\\"matView\\\"\",           -- view matrix\n"
        "    \"RL_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION=\\\"matProjection\\\"\",     -- projection matrix far cull distance\n"
        "    \"RL_DEFAULT_SHADER_UNIFORM_NAME_MODEL=\\\"matModel\\\"\",          -- model matrix\n"
        "    \"RL_DEFAULT_SHADER_UNIFORM_NAME_NORMAL=\\\"matNormal\\\"\",         -- normal matrix (transpose(inverse(matModelView))\n"
        "    \"RL_DEFAULT_SHADER_UNIFORM_NAME_COLOR=\\\"colDiffuse\\\"\",        -- color diffuse (base tint color, multiplied by texture color)\n"
        "    \"RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0=\\\"texture0\\\"\",          -- texture0 (texture slot active 0)\n"
        "    \"RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1=\\\"texture1\\\"\",          -- texture1 (texture slot active 1)\n"
        "    \"RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2=\\\"texture2\\\"\",          -- texture2 (texture slot active 2)\n\n"

        "    --------------------------------------------------------------------------------------\n"
        "    -- Module: rshapes - Configuration Flags\n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Use QUADS instead of TRIANGLES for drawing when possible\n"
        "    -- Some lines-based shapes could still use lines\n"
        "    \"SUPPORT_QUADS_DRAW_MODE=1\",\n\n"

        "    -- rshapes: Configuration values\n"
        "    --\n"
        "    \"SPLINE_SEGMENT_DIVISIONS=24\",       -- Spline segments subdivisions\n\n"

        "    --\n"
        "    -- Module: rtextures - Configuration Flags\n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Selecte desired fileformats to be supported for image data loading\n"
        "    \"SUPPORT_FILEFORMAT_PNG=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_BMP=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_TGA=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_JPG=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_GIF=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_QOI=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_PSD=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_DDS=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_HDR=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_PIC=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_KTX=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_ASTC=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_PKM=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_PVR=1\",\n\n"

        "    -- Support image export functionality (.png, .bmp, .tga, .jpg, .qoi)\n"
        "    \"SUPPORT_IMAGE_EXPORT=1\",\n"
        "    -- Support procedural image generation functionality (gradient, spot, perlin-noise, cellular)\n"
        "    \"SUPPORT_IMAGE_GENERATION=1\",\n"
        "    -- Support multiple image editing functions to scale, adjust colors, flip, draw on images, crop...\n"
        "    -- If not defined, still some functions are supported: ImageFormat(), ImageCrop(), ImageToPOT()\n"
        "    \"SUPPORT_IMAGE_MANIPULATION=1\",\n\n"

        "    --------------------------------------------------------------------------------------\n"
        "    -- Module: rtext - Configuration Flags\n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Default font is loaded on window initialization to be available for the user to render simple text\n"
        "    -- NOTE: If enabled, uses external module functions to load default raylib font\n"
        "    \"SUPPORT_DEFAULT_FONT=1\",\n"
        "    -- Selected desired font fileformats to be supported for loading\n"
        "    \"SUPPORT_FILEFORMAT_TTF=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_FNT=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_BDF=1\",\n\n"

        "    -- Support text management functions\n"
        "    -- If not defined, still some functions are supported: TextLength(), TextFormat()\n"
        "    \"SUPPORT_TEXT_MANIPULATION=1\",\n"
        "    -- On font atlas image generation [GenImageFontAtlas()], add a 3x3 pixels white rectangle\n"
        "    -- at the bottom-right corner of the atlas. It can be useful to for shapes drawing, to allow\n"
        "    -- drawing text and shapes with a single draw call [SetShapesTexture()].\n"
        "    \"SUPPORT_FONT_ATLAS_WHITE_REC=1\",\n\n"

        "    -- rtext: Configuration values\n"
        "    --------------------------------------------------------------------------------------\n"
        "    \"MAX_TEXT_BUFFER_LENGTH = 1024\",       -- Size of internal static buffers used on some functions:\n"
        "                                        -- TextFormat(), TextSubtext(), TextToUpper(), TextToLower(), TextToPascal(), TextSplit()\n"
        "    \"MAX_TEXTSPLIT_COUNT=128\",       -- Maximum number of substrings to split: TextSplit()\n\n"

        "    --------------------------------------------------------------------------------------\n"
        "    -- Module: rmodels - Configuration Flags\n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Selected desired model fileformats to be supported for loading\n"
        "    \"SUPPORT_FILEFORMAT_OBJ=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_MTL=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_IQM=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_GLTF=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_VOX=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_M3D=1\",\n"
        "    -- Support procedural mesh generation functions, uses external par_shapes.h library\n"
        "    -- NOTE: Some generated meshes DO NOT include generated texture coordinates\n"
        "    \"SUPPORT_MESH_GENERATION=1\",\n"

        "    -- rmodels: Configuration values\n\n"
        "    --------------------------------------------------------------------------------------\n"
        "    \"MAX_MATERIAL_MAPS=12\",       -- Maximum number of shader maps supported\n\n"

        "    --#ifdef RL_SUPPORT_MESH_GPU_SKINNING\n"
        "    \"MAX_MESH_VERTEX_BUFFERS=9\",      -- Maximum vertex buffers (VBO) per mesh\n"
        "    --#else\n"
        "    --\"MAX_MESH_VERTEX_BUFFERS=7\",       -- Maximum vertex buffers (VBO) per mesh\n"
        "    --#endif\n\n"

        "    --------------------------------------------------------------------------------------\n"
        "    -- Module: raudio - Configuration Flags\n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Desired audio fileformats to be supported for loading\n"
        "    \"SUPPORT_FILEFORMAT_WAV=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_OGG=1\",\n"
        "    \"SUPPORT_FILEFORMAT_MP3=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_QOA=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_FLAC=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_XM=1\",\n"
        "    --\"SUPPORT_FILEFORMAT_MOD=1\",\n\n"

        "    -- raudio: Configuration values\n"
        "    --------------------------------------------------------------------------------------\n"
        "    \"AUDIO_DEVICE_FORMAT=ma_format_f32\",    -- Device output format (miniaudio: float-32bit)\n"
        "    \"AUDIO_DEVICE_CHANNELS=2\",    -- Device output channels: stereo\n"
        "    \"AUDIO_DEVICE_SAMPLE_RATE=0\",    -- Device sample rate (device default)\n\n"

        "    \"MAX_AUDIO_BUFFER_POOL_CHANNELS=16\",    -- Maximum number of audio pool channels\n\n"

        "    --------------------------------------------------------------------------------------\n"
        "    -- Module: utils - Configuration Flags\n"
        "    --------------------------------------------------------------------------------------\n"
        "    -- Standard file io library (stdio.h) included\n"
        "    \"SUPPORT_STANDARD_FILEIO=1\",\n"
        "    -- Show TRACELOG() output messages\n"
        "    -- NOTE: By default LOG_DEBUG traces not shown\n"
        "    \"SUPPORT_TRACELOG=1\",\n"
        "    --\"SUPPORT_TRACELOG_DEBUG=1\",\n\n"

        "    -- utils: Configuration values\n"
        "    --------------------------------------------------------------------------------------\n"
        "    \"MAX_TRACELOG_MSG_LENGTH=256\"       -- Max length of one trace-log message\n\n"

        "    -- Enable partial support for clipboard image, only working on SDL3 o\n"
        "    -- being on both Windows OS + GLFW o Windows OS + RGFW\n"
        "    --\"SUPPORT_CLIPBOARD_IMAGE=1\",\n\n"

        "    --#if defined(SUPPORT_CLIPBOARD_IMAGE)\n"
        "        --#ifndef STBI_REQUIRED\n"
        "            --\"STBI_REQUIRED\",\n"
        "        --#endif\n\n"

        "        --#ifndef SUPPORT_FILEFORMAT_BMP -- For clipboard image on Windows\n"
        "            --\"SUPPORT_FILEFORMAT_BMP=1\",\n"
        "        --#endif\n\n"

        "        --#ifndef SUPPORT_FILEFORMAT_PNG -- Wayland uses png for prints, at least it was on 22 LTS ubuntu\n"
        "            --\"SUPPORT_FILEFORMAT_PNG=1\",\n"
        "        --#endif\n\n"

        "        --#ifndef SUPPORT_FILEFORMAT_JPG\n"
        "            --\"SUPPORT_FILEFORMAT_JPG=1\",\n"
        "        --#endif\n\n"

        "        --#ifndef SUPPORT_MODULE_RTEXTURES\n"
        "            --\"SUPPORT_MODULE_RTEXTURES=1\",\n"
        "        --#endif\n"
        "    --#endif\n"
        "}\n";

    std::string RaylibLicense =
        "Copyright (c) 2013-2025 Ramon Santamaria (@raysan5)\n\n"

        "This software is provided \"as - is\", without any express or implied warranty. In no event \n"
        "will the authors be held liable for any damages arising from the use of this software.\n\n"

        "Permission is granted to anyone to use this software for any purpose, including commercial \n"
        "applications, and to alter it and redistribute it freely, subject to the following restrictions:\n\n"

        "  1. The origin of this software must not be misrepresented; you must not claim that you \n"
        "  wrote the original software. If you use this software in a product, an acknowledgment \n"
        "  in the product documentation would be appreciated but is not required.\n\n"

        "  2. Altered source versions must be plainly marked as such, and must not be misrepresented\n"
        "  as being the original software.\n\n"

        "  3. This notice may not be removed or altered from any source distribution.";

    std::string RaylibPremake =
        "local EXTERNAL_CONFIG_FLAGS = include(\"EXTERNAL_CONFIG_FLAGS.lua\")\n\n"
        "project \"raylib\"\n"
        "    kind \"StaticLib\"\n"
        "    language \"C\"\n"
        "    cdialect \"C11\"\n"
        "    staticruntime \"on\"\n\n"

        "    defines {\"_WINSOCK_DEPRECATED_NO_WARNINGS\", \"_CRT_SECURE_NO_WARNINGS\"}\n"
        "    defines {\"PLATFORM_DESKTOP\", \"GRAPHICS_API_OPENGL_43\"}\n\n"

        "    defines {\"EXTERNAL_CONFIG_FLAGS\", EXTERNAL_CONFIG_FLAGS}\n\n"

        "    targetdir (\"../../bin/%{cfg.buildcfg}/%{prj.name}\")\n"
        "    objdir (\"../../bin-int/%{cfg.buildcfg}/%{prj.name}\")\n\n"

        "    files {\n"
        "        \"raylib/src/*.c\",\n"
        "        \"raylib/src/*.h\"\n"
        "    }\n\n"

        "    includedirs {\n"
        "        \"raylib/src\",\n"
        "        \"raylib/src/external/glfw/include\"\n"
        "    }\n\n"

        "    filter {\"system:windows\"}\n"
        "        defines {\"WINDOWS_\"}\n"
        "        systemversion \"latest\"\n\n"

        "        filter {\"toolset:msc*\"}\n"
        "            buildoptions {\"/wd4005\"}\n\n"

        "        filter {\"configurations:Debug\"}\n"
        "            defines {\"DEBUG_\"}\n"
        "            runtime \"Debug\"\n"
        "            optimize \"Off\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Release\"}\n"
        "            defines {\"RELEASE_\"}\n"
        "            runtime \"Release\"\n"
        "            optimize \"On\"\n"
        "            symbols \"On\"\n\n"

        "        filter {\"configurations:Dist\"}\n"
        "            defines {\"DIST_\"}\n"
        "            runtime \"Release\"\n"
        "            optimize \"On\"\n"
        "            symbols \"Off\"\n\n"

        "    filter {}";

    std::string CleanVendor =
        "@echo off\n"
        "setlocal\n\n"

        "pushd \"%~dp0\"\n\n"

        "echo === Clean operation started ===\n\n"

        "set \"PROJECT_DIR=" + InProjectFolder + "\"\n"
        "set \"VENDOR_DIR=Vendor\"\n"
        "set \"PREMAKE_DIR=premake\"\n"
        + (Raylib ? "set \"RAYLIB_DIR=raylib\"\n" : "")
        + (ENet ? "set \"ENET_DIR=enet\"\n" : "")

        + "\npushd \"..\\..\\%PROJECT_DIR%\\%VENDOR_DIR%\"\n"
        + (Raylib ? "pushd \"%RAYLIB_DIR%\"\n\n" : "")

        + (Raylib ? "if exist \"%RAYLIB_DIR%\" (\n" : "")
        + (Raylib ? "    rmdir /S /Q \"%RAYLIB_DIR%\"\n" : "")
        + (Raylib ? "    echo Removed %RAYLIB_DIR%\n" : "")
        + (Raylib ? ")\n\n" : "")

        + (Raylib ? "popd\n" : "")
        + (ENet ? "pushd \"%ENET_DIR%\"\n\n" : "")

        + (ENet ? "if exist \"%ENET_DIR%\" (\n" : "")
        + (ENet ? "    rmdir /S /Q \"%ENET_DIR%\"\n" : "")
        + (ENet ? "    echo Removed %ENET_DIR%\n" : "")
        + (ENet ? ")\n\n" : "")

        + (ENet ? "popd\n" : "")
        + "pushd \"%PREMAKE_DIR%\"\n\n"

        "if exist \"%PREMAKE_DIR%\" (\n"
        "    rmdir /S /Q \"%PREMAKE_DIR%\"\n"
        "    echo Removed %PREMAKE_DIR%\n"
        ")\n\n"

        "popd\n"
        "popd\n"
        "popd\n\n"

        "echo === Clean operation completed ===\n\n"

        "endlocal\n"
        "pause";

    std::string Clean =
        "@echo off\n"
        "setlocal enabledelayedexpansion\n\n"

        "pushd \"%~dp0\"\n\n"

        "echo === Clean operation started ===\n\n"

        "set \"MAIN_DIR=" + InMainFolder + "\"\n"
        "set \"PROJECT_DIR=" + InProjectFolder + "\"\n"
        "set \"EXECUTABLE_DIR=" + InExecutableFolder + "\"\n"
        + (Engine ? "set \"ENGINE_DIR=" + InEngineFolder + "\"\n" : "")
        + "set \"VENDOR_DIR=Vendor\"\n"
        + (Raylib ? "set \"RAYLIB_DIR=raylib\"\n" : "")
        + (ENet ? "set \"ENET_DIR=enet\"\n" : "")
        + "set \"BIN_DIR=bin\"\n"
        "set \"BININT_DIR=bin-int\"\n"
        "set \"VS_DIR=.vs\"\n\n"

        "pushd ..\\..\n\n"

        "if exist \"%VS_DIR%\" (\n"
        "    rmdir /S /Q \"%VS_DIR%\"\n"
        "    echo Removed %VS_DIR%\n"
        ")\n\n"

        "echo Cleaning %MAIN_DIR%...\n"
        "del /F /Q *.sln Makefile 2>nul\n\n"

        "pushd \"%PROJECT_DIR%\"\n\n"

        "for %%D in (\"%BIN_DIR%\" \"%BININT_DIR%\") do (\n"
        "    if exist \"%%~D\" (\n"
        "        rmdir /S /Q \"%%~D\"\n"
        "        echo Removed %%~D\n"
        "    )\n"
        ")\n\n"

        + (Raylib || ENet ? "pushd \"%VENDOR_DIR%\"\n" : "")
        + (Raylib ? "pushd \"%RAYLIB_DIR%\"\n\n" : "")

        + (Raylib ? "echo Cleaning %RAYLIB_DIR%...\n" : "")
        + (Raylib ? "del /F /Q *.vcxproj *.vcxproj.filters *.user Makefile 2>nul\n\n" : "")

        + (Raylib ? "popd\n" : "")
        + (ENet ? "pushd \"%ENET_DIR%\"\n\n" : "")

        + (ENet ? "echo Cleaning %ENET_DIR%...\n" : "")
        + (ENet ? "del /F /Q *.vcxproj *.vcxproj.filters *.user Makefile 2>nul\n\n" : "")

        + (ENet ? "popd\n" : "")
        + (Raylib || ENet ? "popd\n" : "")

        + (Engine ? "pushd \"%ENGINE_DIR%\"\n\n" : "")

        + (Engine ? "echo Cleaning %ENGINE_DIR%...\n" : "")
        + (Engine ? "del /F /Q *.vcxproj *.vcxproj.filters *.user Makefile 2>nul\n\n" : "")

        + (Engine ? "popd\n" : "")
        + "pushd \"%EXECUTABLE_DIR%\"\n\n"

        "echo Cleaning %EXECUTABLE_DIR%...\n"
        "del /F /Q *.vcxproj *.vcxproj.filters *.user Makefile 2>nul\n\n"

        "popd\n"
        "popd\n"
        "popd\n"
        "popd\n\n"

        "echo === Clean operation completed ===\n\n"

        "endlocal\n"
        "pause";

    std::string SetUpVendor =
        "@echo off\n"
        "setlocal\n\n"

        "pushd \"%~dp0\"\n\n"

        "echo === Download operation started ===\n\n"

        "set \"PROJECT_DIR=" + InProjectFolder + "\"\n"
        "set \"VENDOR_DIR=Vendor\"\n"
        "set \"PREMAKE_DIR=premake\"\n"
        + (Raylib ? "set \"RAYLIB_DIR=raylib\"\n" : "")
        + (ENet ? "set \"ENET_DIR=enet\"\n" : "")
        + (Raylib ? "set \"EXAMPLES_DIR=examples\"\n" : "")
        + "set \"PREMAKE_TAG=5.0.0-beta7\"\n"
        + (Raylib ? "set \"RAYLIB_TAG=5.5\"\n" : "")
        + (ENet ? "set \"ENET_TAG=1.3.18\"\n" : "")

        + "\ncurl --version >nul 2>&1 || (\n"
        "    echo Error: curl is not installed.\n"
        "    exit /b 1\n"
        ")\n\n"

        "tar --version >nul 2>&1 || (\n"
        "    echo Error: tar is not installed.\n"
        "    exit /b 1\n"
        ")\n\n"

        "pushd \"..\\..\\%PROJECT_DIR%\\%VENDOR_DIR%\"\n"
        "pushd \"%PREMAKE_DIR%\"\n\n"

        "if exist \"%PREMAKE_DIR%\" (\n"
        "    rmdir /S /Q \"%PREMAKE_DIR%\"\n"
        ")\n\n"

        "mkdir \"%PREMAKE_DIR%\"\n"
        "pushd \"%PREMAKE_DIR%\"\n\n"

        "echo Downloading premake, version %PREMAKE_TAG%...\n"
        "curl -L -o premake-windows.zip \"https://github.com/premake/premake-core/releases/download/v%PREMAKE_TAG%/premake-%PREMAKE_TAG%-windows.zip\"\n"
        "tar -xf premake-windows.zip\n"
        "del premake-windows.zip\n\n"

        "popd\n"
        "popd\n"
        + (Raylib ? "pushd \"%RAYLIB_DIR%\"\n\n" : "")

        + (Raylib ? "if exist \"%RAYLIB_DIR%\" (\n" : "")
        + (Raylib ? "    rmdir /S /Q \"%RAYLIB_DIR%\"\n" : "")
        + (Raylib ? ")\n\n" : "")

        + (Raylib ? "mkdir \"%RAYLIB_DIR%\"\n" : "")
        + (Raylib ? "pushd \"%RAYLIB_DIR%\"\n\n" : "")

        + (Raylib ? "echo Downloading raylib version %RAYLIB_TAG%...\n" : "")
        + (Raylib ? "curl -L -o \"raylib-%RAYLIB_TAG%.tar.gz\" \"https://github.com/raysan5/raylib/archive/refs/tags/%RAYLIB_TAG%.tar.gz\"\n" : "")
        + (Raylib ? "tar -xzf \"raylib-%RAYLIB_TAG%.tar.gz\" --strip-components=1\n" : "")
        + (Raylib ? "del \"raylib-%RAYLIB_TAG%.tar.gz\"\n" : "")
        + (Raylib ? "if exist \"%EXAMPLES_DIR%\" (\n" : "")
        + (Raylib ? "    echo Removing examples directory from raylib...\n" : "")
        + (Raylib ? "    rmdir /S /Q \"%EXAMPLES_DIR%\"\n" : "")
        + (Raylib ? ")\n\n" : "")

        + (Raylib ? "popd\n" : "")
        + (Raylib ? "popd\n" : "")
        + (ENet ? "pushd \"%ENET_DIR%\"\n\n" : "")

        + (ENet ? "if exist \"%ENET_DIR%\" (\n" : "")
        + (ENet ? "    rmdir /S /Q \"%ENET_DIR%\"\n" : "")
        + (ENet ? ")\n\n" : "")

        + (ENet ? "mkdir \"%ENET_DIR%\"\n" : "")
        + (ENet ? "pushd \"%ENET_DIR%\"\n\n" : "")

        + (ENet ? "echo Downloading enet version %ENET_TAG%...\n" : "")
        + (ENet ? "curl -L -o \"enet-%ENET_TAG%.tar.gz\" \"https://github.com/lsalzman/enet/archive/refs/tags/v%ENET_TAG%.tar.gz\"\n" : "")
        + (ENet ? "tar -xzf \"enet-%ENET_TAG%.tar.gz\" --strip-components=1\n" : "")
        + (ENet ? "del \"enet-%ENET_TAG%.tar.gz\"\n" : "")

        + (ENet ? "popd\n" : "")
        + (ENet ? "popd\n" : "")
        + "popd\n"
        "popd\n\n"

        "echo === Download operation completed ===\n\n"

        "endlocal\n"
        "pause";

    std::string MinGW =
        "@echo off\n\n"

        "pushd \"%~dp0\"\n\n"

        "pushd ..\\..\n\n"

        + InProjectFolder + "\\Vendor\\premake\\premake\\premake5.exe gmake\n\n"

        "popd\n"
        "popd\n\n"

        "pause";

    std::string MSVC =
        "@echo off\n\n"

        "pushd \"%~dp0\"\n\n"

        "pushd ..\\..\n\n"

        + InProjectFolder + "\\Vendor\\premake\\premake\\premake5.exe vs2022\n\n"

        "popd\n"
        "popd\n\n"

        "pause";

    if (!CreateFolder(MainFolder)) {
        return 1;
    }
    if (!CreateFolder(MainFolder / ProjectFolder)) {
        return 1;
    }
    if (Icon) {
        if (!CreateFolder(MainFolder / ProjectFolder / "assets")) {
            return 1;
        }
        if (!CreateFolder(MainFolder / ProjectFolder / "assets" / "icon")) {
            return 1;
        }
    }
    if (Engine) {
        if (!CreateFolder(MainFolder / ProjectFolder / EngineFolder)) {
            return 1;
        }
        if (!CreateFolder(MainFolder / ProjectFolder / EngineFolder / "include")) {
            return 1;
        }
        if (!CreateFolder(MainFolder / ProjectFolder / EngineFolder / "src")) {
            return 1;
        }
    }
    if (!CreateFolder(MainFolder / ProjectFolder / ExecutableFolder)) {
        return 1;
    }
    if (!CreateFolder(MainFolder / ProjectFolder / ExecutableFolder / "include")) {
        return 1;
    }
    if (!CreateFolder(MainFolder / ProjectFolder / ExecutableFolder / "src")) {
        return 1;
    }
    if (!CreateFolder(MainFolder / ProjectFolder / "Vendor")) {
        return 1;
    }
    if (ENet) {
        if (!CreateFolder(MainFolder / ProjectFolder / "Vendor" / "enet")) {
            return 1;
        }
    }
    if (!CreateFolder(MainFolder / ProjectFolder / "Vendor" / "premake")) {
        return 1;
    }
    if (Raylib) {
        if (!CreateFolder(MainFolder / ProjectFolder / "Vendor" / "raylib")) {
            return 1;
        }
    }
    if (!CreateFolder(MainFolder / "Scripts")) {
        return 1;
    }
    if (!CreateFolder(MainFolder / "Scripts" / "Clean")) {
        return 1;
    }
    if (!CreateFolder(MainFolder / "Scripts" / "SetUp")) {
        return 1;
    }

    if (!CreateFile(MainFolder / ".gitignore", GitIgnore)) {
        return 1;
    }
    if (!CreateFile(MainFolder / "premake5.lua", ProjectPremake)) {
        return 1;
    }
    if (Icon) {
        if (!CreateFile(MainFolder / ProjectFolder / "assets" / "icon" / "resource.rc", Resource)) {
            return 1;
        }
    }
    if (Engine) {
        if (!CreateFile(MainFolder / ProjectFolder / EngineFolder / "premake5.lua", EnginePremake)) {
            return 1;
        }
        if (!CreateFile(MainFolder / ProjectFolder / EngineFolder / "include" / "pch.hpp", "")) {
            return 1;
        }
        if (!CreateFile(MainFolder / ProjectFolder / EngineFolder / "src" / "pch.cpp", pch)) {
            return 1;
        }
    }
    if (!CreateFile(MainFolder / ProjectFolder / ExecutableFolder / "premake5.lua", ExecutablePremake)) {
        return 1;
    }
    if (!CreateFile(MainFolder / ProjectFolder / ExecutableFolder / "src" / "main.cpp", main)) {
        return 1;
    }
    if (ENet) {
        if (!CreateFile(MainFolder / ProjectFolder / "Vendor" / "enet" / "LICENSE.txt", ENetLicense)) {
            return 1;
        }
        if (!CreateFile(MainFolder / ProjectFolder / "Vendor" / "enet" / "premake5.lua", ENetPremake)) {
            return 1;
        }
    }
    if (!CreateFile(MainFolder / ProjectFolder / "Vendor" / "premake" / "LICENSE.txt", PremakeLicense)) {
        return 1;
    }
    if (Raylib) {
        if (!CreateFile(MainFolder / ProjectFolder / "Vendor" / "raylib" / "EXTERNAL_CONFIG_FLAGS.lua", EXTERNAL_CONFIG_FLAGS)) {
            return 1;
        }
        if (!CreateFile(MainFolder / ProjectFolder / "Vendor" / "raylib" / "LICENSE.txt", RaylibLicense)) {
            return 1;
        }
        if (!CreateFile(MainFolder / ProjectFolder / "Vendor" / "raylib" / "premake5.lua", RaylibPremake)) {
            return 1;
        }
    }
    if (!CreateFile(MainFolder / "Scripts" / "Clean" / "Clean-Vendor.bat", CleanVendor)) {
        return 1;
    }
    if (!CreateFile(MainFolder / "Scripts" / "Clean" / "Clean.bat", Clean)) {
        return 1;
    }
    if (!CreateFile(MainFolder / "Scripts" / "SetUp" / "SetUp-Vendor.bat", SetUpVendor)) {
        return 1;
    }
    if (!CreateFile(MainFolder / "Scripts" / "SetUp" / "SetUpForMinGW.bat", MinGW)) {
        return 1;
    }
    if (!CreateFile(MainFolder / "Scripts" / "SetUp" / "SetUpForMSVC.bat", MSVC)) {
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\nDone: (" << ms.count() << "ms)\n";
    std::cout << "==============================";

    std::cin.get();
}