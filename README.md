# The Atlas Engine

[![✅CI](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml/badge.svg)](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml)
[![GitHub stars](https://img.shields.io/github/stars/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/network)
[![GitHub issues](https://img.shields.io/github/issues/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/issues)

Open-source Vulkan 3D game engine with native C++20 modules support.

Using **Conan** to manage our third party dependencies with Ninja + CMake for our build system.

<img width="1505" height="902" alt="Screenshot 2026-03-15 134559" src="https://github.com/user-attachments/assets/170a9ffe-4fd7-440a-93e0-c2d3172ce928" />

# Major Highlights
* HDRI Environment Map Support
* Native Support for C++20 Modules
* Interactive UI Editor

# Setup Development Environment

New here? We use the C++ package manager, Conan. Follow the [Getting Started Guide](https://engine3d-dev.github.io/0.1/getting_started/) to setup your environment.

## How to Build the Editor

Since there isn't a way to officially to building the editor. In the CMakeLists.txt file.

Just uncomment the `editor` into your CMakeLists.txt file as shown in this example to build the editor.

```Cmake
add_subdirectory(editor)
```

## Building the Project

> [!TIP]
> `-s build_type=Debug` is recommended to compile the project as a Debug build for development.
> For development to disable the testing environment.

```
conan atlas build . -s build_type=Debug -o enable_tests_only=False
```

## Executable Location

After building TheAtlasEngine has been successful. Your executable will be executed via the following path layout.

* Debug build will follow with, `./build/Debug/<bin>`
* Release build will follow with, `./build/Release/<bin>`

Example on Windows, you'd execute the executable in the terminal as:

```Powershell
.\build\Debug\editor\editor.exe
```

# Games Built using TheAtlasEngine

These are games that have been built by me and others to showcase the capabilities of the game engine. Demonstrating capabilities and progression made by the team.

> [!NOTE]
> TheAtlasEngine is quite early in its development, therefore some of the games may be buggy or have missing features in the games released.
> We believe to test the capabilities of TheAtlasEngine is to continue and building games with it by participating in game jams to learn what works for the project.


## Monkey Ball

### Description
This was the first game that we made with TheAtlasEngine when the project first started.

<img width="1238" height="631" alt="Screenshot 2025-12-06 025821" src="https://github.com/user-attachments/assets/b1d78746-4f1b-41db-91ae-cb84299e1f54" />

## Wallace and Grommit Rocket Game
### Description
Developed during the SFSU Game Dev Club Spring 2025 Game Jam, this project serves as a technical demonstration of TheAtlasEngine core capabilities. Inspired by similar projects from creators like TheCherno -- to test the game engine's capabilities.

* [itch.io link](https://niccoll-dyson.itch.io/wallace-and-grommit-spaceship-game) to out game jam submission
* GitHub [repository link](https://github.com/SpinnerX/3d-rocket-game)

![Screenshot 2025-03-03 010511](https://github.com/user-attachments/assets/f3758e33-8d4b-4fa8-af08-167bca14037f)
