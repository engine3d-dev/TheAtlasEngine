from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.files import copy
import os

class Editor(ConanFile):
    name = "editor"
    version = "1.0"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "Editor/*"

    def build_requirements(self):
        self.tool_requires("make/4.4.1")
        self.tool_requires("cmake/3.27.1")
        self.tool_requires("engine3d-cmake-utils/1.0")

    def requirements(self):
        self.requires("glfw/3.4", transitive_headers=True)
        self.requires("opengl/system", transitive_headers=True)
        # These end in 1.0 because they are engine3d-customized conan packages
        # Slighly modified of the conan packages and it's CMake generators to using "Unix Makefiles" 
        self.requires("fmt/10.2.1", transitive_headers=True)
        self.requires("spdlog/1.14.1", transitive_headers=True)
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("yaml-cpp/0.8.0", transitive_headers=True)
        self.requires("box2d/2.4.2")
        self.requires("imguidocking/1.0")
        self.requires("entt/3.13.2")


        self.requires("joltphysics/1.0")
        
        self.requires("vulkan-headers/1.3.290.0")
        if self.settings.os == "Linux":
            self.requires("vulkan-loader/1.3.290.0")
        print(f"OS = {self.settings.os}")

        self.requires("engine3d/1.0")

    def generate(self):
        cmake = CMakeDeps(self)
        cmake.generate()
        tc = CMakeToolchain(self, generator="Unix Makefiles")
        tc.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def layout(self):
        cmake_layout(self)
