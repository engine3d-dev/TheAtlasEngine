# engine3d
Open-source game engine to be used for developing games and working on cool projects.

## Prerequisites
To learn more about how to install the prerequisites before building engine3d go to the website `getting started` tab

## Clone Repository
These steps assume you've already done the prerequisites step before building engine3d.
```
git clone https://github.com/engine3d-dev/engine3d
cd engine3d
```

## Build using Conan
`conan create .` will build engine3d and add that to your conan cache.

`conan build .` - Once you do `conan create`, you can simply use `conan build` to build the project

## Running Engine3D
`.\build\Release\Editor\Editor.exe` - At the current time of Engine3D's development, this is how you would execute the editor.
