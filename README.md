# The Atlas Engine

[![✅CI](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml/badge.svg)](https://github.com/engine3d-dev/TheAtlasEngine/actions/workflows/clang-format-deploy.yml)
[![GitHub stars](https://img.shields.io/github/stars/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/network)
[![GitHub issues](https://img.shields.io/github/issues/engine3d-dev/TheAtlasEngine.svg)](https://github.com/engine3d-dev/TheAtlasEngine/issues)

Open-source game engine for making games, building cool projects, and doing cool work in building features with the satisfaction of using them during game jams and other purposes.

## How to build the engine

> [!TIP]
> Before you build the project make sure to follow the setup instruction in the [getting start](https://engine3d-dev.github.io/getting_started) page.

To get started in contributing here are the steps to do so. Follow these instructions.

### Clone the repository

```
git clone https://github.com/engine3d-dev/TheAtlasEngine
cd TheAtlasEngine
```

### Building using Conan

To build the project, run the following command below.

> [!TIP]
> `-b missing` is used to get the dependency's the project uses before building the project itself. To load that into conan cache for reusability.

```
conan build . -b missing
```

## Running the engine

When running the project. The engine will compile with the editor project within the repository.

Conan the C++ package manager will compile your executable into the build directory it generates you.

You should be able to find it in the `./build/editor/editor`.
