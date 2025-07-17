![Project Builder](Resources/Project-Builder_Banner.png)

## `prj.exe` quickly creates C++ projects with a clean and consistent structure.

<pre>
<span style="color:yellow;">main</span>
├── <span style="color:yellow;">project</span>
│   ├── <span style="color:yellow;">assets</span> <span style="color:white;">(optional)</span>
│   │   └── <span style="color:yellow;">icon</span> <span style="color:white;">(here you add icon.ico)</span>
│   │       └── <span style="color:lightgray;">resource.rc</span>
│   ├── <span style="color:yellow;">exe</span>
│   │   ├── <span style="color:yellow;">include</span>
│   │   ├── <span style="color:yellow;">src</span>
│   │   │   └── <span style="color:cyan;">main.cpp</span>
│   │   └── <span style="color:orange;">premake5.lua</span>
│   ├── <span style="color:yellow;">engine</span> <span style="color:white;">(optional)</span>
│   │   ├── <span style="color:yellow;">include</span>
│   │   │   └── <span style="color:magenta;">pch.hpp</span>
│   │   ├── <span style="color:yellow;">src</span>
│   │   │   └── <span style="color:cyan;">pch.cpp</span>
│   │   └── <span style="color:orange;">premake5.lua</span>
│   └── <span style="color:yellow;">Vendor</span>
│       ├── <span style="color:yellow;">enet</span> <span style="color:white;">(optional)</span>
│       │   ├── <span style="color:dodgerblue;">LICENSE.txt</span>
│       │   └── <span style="color:orange;">premake5.lua</span>
│       ├── <span style="color:yellow;">premake</span>
│       │   └── <span style="color:dodgerblue;">LICENSE.txt</span>
│       └── <span style="color:yellow;">raylib</span> <span style="color:white;">(optional)</span>
│           ├── <span style="color:orange;">EXTERNAL_CONFIG_FLAGS.lua</span>
│           ├── <span style="color:dodgerblue;">LICENSE.txt</span>
│           └── <span style="color:orange;">premake5.lua</span>
├── <span style="color:yellow;">Scripts</span>
│   ├── <span style="color:yellow;">Clean</span>
│   │   ├── <span style="color:lime;">Clean.bat</span>
│   │   └── <span style="color:lime;">Clean-Vendor.bat</span>
│   └── <span style="color:yellow;">SetUp</span>
│       ├── <span style="color:lime;">SetUpForMinGW.bat</span>
│       ├── <span style="color:lime;">SetUpForMSVC.bat</span>
│       └── <span style="color:lime;">SetUp-Vendor.bat</span>
├── <span style="color:gray;">.gitignore</span>
└── <span style="color:orange;">premake5.lua</span>
</pre>
## 🚀Features

- Choose folder names (`main`, `project`, `exe`, `engine` optional)
- Optional:
  - Icon support
  - Raylib linked
  - ENet linked
  - Engine Style (`.lib` separation with pch.hpp)
- Setup scripts to:
  - Download dependencies (Premake, Raylib, ENet)
  - Generate project files for MSVC or MinGW
- Clean scripts to:
  - Remove generated files
  - Remove downloaded dependencies

## 🛠️How to Use

1. Run `prj.exe`
2. Follow the prompts to configure your project
3. Run the setup scripts to prepare the project (start with setup vendor)

![Example Usage](Resources/Project-Builder.png)

## 📦Requirements

- Git
- Curl and Tar
- C++ Compiler (MSVC or MinGW)