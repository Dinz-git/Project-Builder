![Project Builder](Resources/Project-Builder_Banner.png)

`prj.exe` quickly creates C++ projects with a clean and consistent structure.

## Features

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

## How to Use

1. Run `prj.exe`
2. Follow the prompts to configure your project
3. Run the setup scripts to prepare the project (start with setup vendor)

![Example Usage](Resources/Project-Builder.png)

## Requirements

- Git
- Curl and Tar
- C++ Compiler (MSVC or MinGW)