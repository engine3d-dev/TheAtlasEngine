from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class GameDemo(ConanFile):
    name = "game-demo"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    export_source = "CMakeLists.txt", "application.cpp"

    # Putting all of your build-related dependencies here
    def build_requirements(self):
        self.tool_requires("make/4.4.1")
        self.tool_requires("cmake/3.27.1")
        self.tool_requires("engine3d-cmake-utils/3.0")

    # Putting all of your packages here
    # To build engine3d/1.0 locally do the following:
    # conan create . --name=engine3d --version=0.1.0 --user=local --channel=12345
    def requirements(self):
        self.requires("flecs/4.0.0")
        self.requires("tinyobjloader/2.0.0-rc10")
        self.requires("stb/cci.20230920")
        self.requires("imguidocking/2.0")
        self.requires("engine3d/1.0@local/2.0")

        # measuring vtable and how do you do thta.

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def layout(self):
        cmake_layout(self)