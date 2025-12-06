# The Atlas Engine

[![✅CI](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml/badge.svg)](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml)
[![GitHub stars](https://img.shields.io/github/stars/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/network)
[![GitHub issues](https://img.shields.io/github/issues/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/issues)

Open-source custom C++ 3D game engine which uses Vulkan for building games.

# Setup Development Environment

TheAtlasEngine uses Conan the C++ package manager to manage our dependencies.

Before you build the project, make sure to check the [getting start](https://engine3d-dev.github.io/0.1/getting_started/) page.

## How to Build the Editor

Since there is not a way to set for building the editor. In the CMakeLists.txt file.

Just add `editor` into your CMakeLists.txt file as shown in this example to build the editor.

```Cmake
build_core_library(
    DIRECTORIES src editor
    # ...
)
```

## Building the Project

> [!TIP]
> `-b missing` is to install any missing dependencies necessary to build TheAtlasEngine successfully.
> `-s build_type=Debug` is to compile as a debug build when developing on the project.

```
conan build . -b missing -s build_type=Debug
```

## Executable Location

After building TheAtlasEngine and the dependencies have been built successful. These are three locations where the editor executable is located at.

* Debug build will follow with, `./build/Debug/<exe path>`
* Release build will follow with, `./build/Release/<exe path>`

Example on Windows, you'd execute the executable in the terminal as:

```Powershell
.\build\Debug\editor\editor.exe
```

# Games Built with TheAtlasEngine

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
Part of the SFSU Game Dev Club's Game Jam of Spring 2025. We developed a flappy-bird-like game mechanic rocket game. This is inspired by a few youtubers who also made a similar game. One of the YouTuber's named TheCherno.

* [link](https://niccoll-dyson.itch.io/wallace-and-grommit-spaceship-game) to our itch.io submission
* [link](https://github.com/SpinnerX/3d-rocket-game) to the GitHub repository

![Screenshot 2025-03-03 010511](https://github.com/user-attachments/assets/f3758e33-8d4b-4fa8-af08-167bca14037f)