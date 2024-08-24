# Engine3d
Open-source game engine to be used for developing games and working on cool projects.

## Prerequisites

First before cloning Engine3D and building the project.

Go through the installation steps at engine3d's website [getting start](https://engine3d-dev.github.io/getting_started)

## Clone Repository
These steps assume you've already done the prerequisites step before building engine3d.
```
git clone https://github.com/engine3d-dev/engine3d
cd engine3d
```

## Building Engine3D using Conan
`conan create . -b missing` will build engine3d and all of its dependencies into your conan cache. `-b missing` means build with any missing dependencies that may be missing in your conan cache.

`conan build .` - After you do `conan create`, you can just build using the `conan build <directory>` to build engine3d.

## Running Engine3D

Your executable will be in the directory `./build/Release/Editor/Editor.exe`.

If you run in debug mode then change the directory name from `Release` to `Debug`.

Example of the changes would be executed, `./build/Debug/Editor/Editor.exe`
