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
        # self.requires("getopt-for-visual-studio/20200201")
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
    
    # def package(self):
    #     copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
    #     copy(self, pattern="*.h", src=os.path.join(self.source_folder, "./Editor/"), dst=os.path.join(self.package_folder, "Editor"))
    #     copy(self, pattern="*.cpp", src=os.path.join(self.source_folder, "./Editor/"), dst=os.path.join(self.package_folder, "Editor"))
    #     copy(self, pattern="*.a", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
    #     copy(self, pattern="*.so", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
    #     copy(self, pattern="*.lib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
    #     copy(self, pattern="*.dll", src=self.build_folder, dst=os.path.join(self.package_folder, "bin"), keep_path=False)
    #     copy(self, pattern="*.dylib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
    #     cmake = CMake(self)
    #     cmake.install()
    
    # def package_info(self):
    #     self.cpp_info.set_property("cmake_target_name", "engine3d::engine3d")
    #     self.cpp_info.libs = ["engine3d"]
    #     self.cpp_info.includedirs = ['./', './engine3d']  # Ordered list of include paths
    