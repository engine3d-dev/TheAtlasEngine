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

    def requirements(self):
        self.requires("imguidocking/1.0")
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