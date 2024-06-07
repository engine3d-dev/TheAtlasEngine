from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.files import copy
import os

class engine3dRecipe(ConanFile):
    name = "engine3d"
    version = "1.0"
    package_type = "library"

    # Optional metadata
    # license = "<Put the package license here>"
    # author = "<Put your name here> <And your email here>"
    # url = "<Package recipe repository url here, for issues about the package>"
    # description = "<Description of engine3d package here>"
    # topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/CMakeLists.txt", "src/engine3d/*", "engine3d/*", "engine3d/Core/*", "engine3d/Event/*"

    def requirements(self):
        self.tool_requires("cmake/3.27.1")
        self.requires("glfw/3.4", transitive_headers=True)
        self.requires("glad/0.1.36", transitive_headers=True)
        self.requires("fmt/10.2.1", transitive_headers=True)
        self.requires("spdlog/1.14.1", transitive_headers=True)
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("yaml-cpp/0.8.0", transitive_headers=True)
        self.requires("box2d/2.4.1", transitive_headers=True)
        # self.requires("joltphysics/3.0.1", transitive_headers=True)
        # self.requires("assimp/5.2.2")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        copy(self, pattern="*.h", src=os.path.join(self.source_folder, "engine3d"), dst=os.path.join(self.package_folder, "engine3d"))
        copy(self, pattern="*.a", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.so", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.lib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.dll", src=self.build_folder, dst=os.path.join(self.package_folder, "bin"), keep_path=False)
        copy(self, pattern="*.dylib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        cmake = CMake(self)
        cmake.install()
    # def package(self):
    #     copy(self, "LICENSE", dst=os.path.join(
    #         self.package_folder, "licenses"), src=self.source_folder)
    #     copy(self, "*.h", dst=os.path.join(self.package_folder, "engine3d"),
    #          src=os.path.join(self.source_folder, "engine3d"))
    #     copy(self, "*.hpp", dst=os.path.join(self.package_folder,
    #          "engine3d"), src=os.path.join(self.source_folder, "engine3d"))

    def package_info(self):
        # self.cpp_info.
        # self.cpp_info.set_property("cmake_target_name", "libhal::stm32f1")
        self.cpp_info.set_property("cmake_target_name", "engine3d::engine3d")
        self.cpp_info.libs = ["engine3d"]
        self.cpp_info.includedirs = ['./', './engine3d']  # Ordered list of include paths

    

    

