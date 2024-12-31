from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.files import copy
import os
from conan.tools.build import can_run
# from conan.tools.os import rm, rmdir
# from os import rmdir, rm

class engine3dRecipe(ConanFile):
    name = "engine3d"
    version = "1.0"
    package_type = "library"
    license = "Apache-2.0"
    homepage = "https://github.com/engine3d-dev/engine3d"
    description = "Multipurpose 3D engine"
    topics = ("Engine", "Graphics", "Animations", "Simulations")
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/CMakeLists.txt", "Editor/CMakeLists.txt", "src/*", "engine3d/*", "Editor/*"

    def build_requirements(self):
        self.tool_requires("make/4.4.1")
        self.tool_requires("cmake/3.27.1")
        self.tool_requires("engine3d-cmake-utils/2.0")

    def requirements(self):
        self.requires("glfw/3.4", transitive_headers=True)
        self.requires("fmt/10.2.1", transitive_headers=True)
        self.requires("spdlog/1.14.1", transitive_headers=True)
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("yaml-cpp/0.8.0", transitive_headers=True)
        self.requires("box2d/2.4.1", transitive_headers=True)
        self.requires("opengl/system", transitive_headers=True)
        self.requires("imguidocking/2.0")

        # Vulkan-related headers and includes packages
        self.requires("vulkan-headers/1.3.290.0", transitive_headers=True)
        self.requires("entt/3.13.2")
        self.requires("tinyobjloader/2.0.0-rc10")
        self.requires("joltphysics/1.0")

        self.requires("boost-ext-ut/2.1.0")

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
        # cmake.verbose = True
        cmake.configure()
        cmake.build()

    def post_build(self):
        print("Running Post Build!")


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

    def package_info(self):
        self.cpp_info.set_property("cmake_target_name", "engine3d::engine3d")
        self.cpp_info.libs = ["engine3d"]
        self.cpp_info.includedirs = ['./', './engine3d']  # Ordered list of include paths

    

    

