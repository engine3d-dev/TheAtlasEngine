# The Atlas Engine

[![✅CI](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml/badge.svg)](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml)
[![GitHub stars](https://img.shields.io/github/stars/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/network)
[![GitHub issues](https://img.shields.io/github/issues/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/issues)

Open-source game engine for making games, building cool projects, and doing cool work in building features with the satisfaction of using them during game jams and other purposes.

## Setup Development Environment

TheAtlasEngine uses Conan the C++ package manager to manage our dependencies.

To setup the development environment, check the [getting start](https://engine3d-dev.github.io/0.1/getting_started/) page.

### How to Build the Editor

Since there is not a way to set for building the editor. In the CMakeLists.txt file.

Just add `editor` into your CMakeLists.txt file as shown in this example to build the editor.

```Cmake
build_core_library(
    DIRECTORIES src editor
    # ...
)
```

### Building the Project

> [!TIP]
> `-b missing` is to install any missing dependencies necessary to build TheAtlasEngine successfully.
> `-s build_type=Debug` is to compile as a debug build when developing on the project.

```
conan build . -b missing -s build_type=Debug
```

### Executable Location

After building TheAtlasEngine and the dependencies have been built successful. These are three locations where the editor executable is located at.

* Debug build will follow with, `./build/Debug/<exe path>`
* Release build will follow with, `./build/Release/<exe path>`

Example on Windows, you'd execute the executable in the terminal as:

```Powershell
.\build\Debug\editor\editor.exe
```

