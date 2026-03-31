# The Atlas Engine
[![C++](https://img.shields.io/badge/C%2B%2B-20%2F23-blue)]()
[![✅CI](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml/badge.svg)](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml)
[![GitHub stars](https://img.shields.io/github/stars/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/network)
[![GitHub issues](https://img.shields.io/github/issues/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/issues)

Open-source Vulkan 3D game engine with native C++20 modules support.

<img width="1505" height="902" alt="Screenshot 2026-03-15 134559" src="https://github.com/user-attachments/assets/170a9ffe-4fd7-440a-93e0-c2d3172ce928" />

## 🧩 Major Highlights
* HDRI Environment Map Support
* Native Support for C++20 Modules
* Interactive UI Editor
* 3D Rigid Body Physics Simulation (Collisions, Shape Colliders)

## 📦 Build System

| Tool | Version | Description |
| :--- | :--- | :--- |
| **[CMake](https://cmake.org/)** | 4.0+ | Major build system for configuration and build generation |
| **[Ninja](https://ninja-build.org/)** | 1.13+ | Small, high-speed build execution |
| **[Conan](https://conan.io/)** | 2.23+ | C/C++ Package Manager for dependency resolution |

## 🗺️ Project Dependencies

These are dependencies to make it developing TheAtlasEngine possible

| Library | Version | Description |
| :--- | :--- | :--- |
| **[GLFW](https://www.glfw.org/)** | 3.4+ | Multi-platform library for OpenGL, OpenGL ES, and Vulkan window/surface management. |
| **[GLM](https://github.com/g-truc/glm)** | 1.0.1+ | A header-only C++ mathematics library for graphics software based on GLSL specifications. |
| **[Dear ImGui](https://github.com/ocornut/imgui)** | 1.92.6+ | Bloat-free Immediate Mode Graphical User Interface for real-time tooling and debugging. |
| **[JoltPhysics](https://github.com/jrouwe/JoltPhysics)** | 5.5.0+ | Multi-core friendly rigid body physics and collision detection library. Written in C++ Suitable for games. |
| **[vulkan-cpp](https://github.com/engine3d-dev/vulkan-cpp)** | 6.0+ | Custom modern C++ abstraction layer using C++20 to develop and simplify using the Vulkan graphics API. |

## 🛠️ Setup Development Environment

New here? Follow the [Getting Started](https://engine3d-dev.github.io/0.1/getting_started/) page to setup the dev environment.

### 🔩 Building the Project

> [!TIP]
> `-s build_type=Debug` is recommended to compile the project as a Debug build for development.
> For development to disable the testing environment.

Since there isn't a way to officially to building the editor. In the CMakeLists.txt file.

Just uncomment the `editor` into your CMakeLists.txt file as shown in this example to build the editor.

```Cmake
add_subdirectory(editor)
```

Then build using the following commands:

```
conan atlas build . -s build_type=Debug -o enable_tests_only=False
```


### 💾 Executing the Editor

After building TheAtlasEngine has been successful. Your executable will be executed via the following path layout.

* Debug build will follow with, `./build/Debug/<bin>`
* Release build will follow with, `./build/Release/<bin>`

Example on Windows, you'd execute the executable in the terminal as:

```Powershell
.\build\Debug\editor\editor.exe
```

## 🎮 Games Built using TheAtlasEngine

These games were build by me (Aaron) and others to demonstrate the earlier capabilities of TheAtlasEngine made by the team.

> [!NOTE]
> TheAtlasEngine was quite early in its development when these games were first built, therefore having some definite flaws and missing features.
> We believe being able to build games and continue participating in game jams will help us continue to learn what works best for TheAtlasEngine.

## Monkey Ball

This Monkey Ball game was the first ever game built using TheAtlasEngine, in its the early development. This game was for the SFSU Game Development Club's end of the semester showcase of Fall 2024.

<img width="1238" height="631" alt="Screenshot 2025-12-06 025821" src="https://github.com/user-attachments/assets/b1d78746-4f1b-41db-91ae-cb84299e1f54" />

## Wallace and Grommit Rocket Game

This game was developed during the SFSU Game Dev Club Spring 2025 Game Jam, this project serves as a demonstration of TheAtlasEngine earlier capabilities. Inspired by similar projects from youtube creators like TheCherno.

* [itch.io link](https://niccoll-dyson.itch.io/wallace-and-grommit-spaceship-game) to checkout the game jam submission.
* GitHub [repository](https://github.com/SpinnerX/3d-rocket-game) link.

![Screenshot 2025-03-03 010511](https://github.com/user-attachments/assets/f3758e33-8d4b-4fa8-af08-167bca14037f)
