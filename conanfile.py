from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy
import os
from pathlib import Path
import shutil
from conan.tools.files import save

class AtlasRecipe(ConanFile):
    name = "atlas"
    version = "0.5"
    package_type = "library"
    license = "Apache-2.0"
    homepage = "https://github.com/engine3d-dev/TheAtlasEngine"
    description = "Multipurpose 3D engine"
    topics = ("Engine", "Graphics", "Animations", "Simulations")
    
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Specifying our build_type is only Debug and Release
    options = {"shared": [True, False], "fPIC": [True, False], "enable_tests_only": [True, False], "enable_shaderc": [True, False]}
    default_options = {"shared": False, "fPIC": True, "enable_tests_only": True, "enable_shaderc": False}
    exports_sources = "atlas/*", "tests/*", "CMakeLists.txt", "LICENSE"

    def build_requirements(self):
        self.tool_requires("cmake/4.1.1")
        self.tool_requires("ninja/1.13.1")
        self.tool_requires("engine3d-cmake-utils/5.0")

    def requirements(self):
        self.requires("joltphysics/5.2.0")
        if self.options.enable_shaderc:
            self.requires("shaderc/2025.3")
        self.requires("imguidocking/3.0")
        self.requires("flecs/4.0.4")
        self.requires("glfw/3.4")
        self.requires("spdlog/1.16.0")
        self.requires("glm/1.0.1")
        self.requires("yaml-cpp/0.8.0")

        # Vulkan-related headers and includes packages
        self.requires("vulkan-cpp/5.0")
        self.requires("tinyobjloader/2.0.0-rc10")
        self.requires("stb/cci.20230920")

        self.requires("nfd/3.0")
        self.requires("watcher/0.12.0")

        if self.options.enable_tests_only:
            self.requires("boost-ext-ut/2.3.1")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        print(f"Current build_type = {self.settings.build_type}")
        if not self.settings.get_safe("build_type"):
            self.settings.build_type = "Debug"
        
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        llvm_path = self.dependencies.build["llvm-toolchain"].package_folder
        cmake_path = self.dependencies.build["cmake"].package_folder
        cmake_binary_location = f"{cmake_path.replace("\\", "/")}/bin/cmake.exe"
        clang_format_path = f"{llvm_path}/bin/clang-format.exe"
        clang_tidy_path = f"{llvm_path}/bin/clang-tidy.exe"
        
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        # TODO: Remove this once fixing shaderc issue in the CI is resolved
        # These are options that can be enabled/disabled with the `-o` parameter when compiling with `conan` command
        tc.generator = "Ninja"
        tc.variables["USE_SHADERC"] = self.options.enable_shaderc
        tc.variables["ENABLE_TESTS_ONLY"] = self.options.enable_tests_only
        tc.variables["CLANG_FORMAT_PATH"] = clang_format_path.replace("\\", "/")
        tc.variables["CLANG_TIDY_PATH"] = clang_tidy_path.replace("\\", "/")
        tc.variables["CMAKE_PATH"] = cmake_binary_location
        tc.generate()

    def build(self):
        # Removing the .gcda files for specifically tests
        # Due to some arc-tag
        # gcda_tests_dir = os.path.join(self.build_folder, "CMakeFiles/unit_test.dir/tests")
        # tests_dir = os.path.join(self.source_folder, "tests");
        # tests_files = os.path.join(tests_dir, "*.test.cpp")

        # gcda_files = os.path.join(gcda_tests_dir, "*.gcda")

        # if os.path.exists(gcda_tests_dir):
        #     for file in glob.glob(gcda_files):
        #         os.remove(file)
        unit_test_dir = os.path.join(self.build_folder, "CMakeFiles\engine3d_unit_test.dir")
        print(f"unit test dir === {unit_test_dir}")

        if os.path.exists(unit_test_dir):
            print("Cleaning up unit_test directory cache")
            shutil.rmtree(unit_test_dir)

        cmake = CMake(self)
        cmake.configure()
        cmake.build()


    def package(self):
        cmake = CMake(self)
        cmake.install()

        copy(self, "LICENSE",
             dst=Path(self.package_folder) / "licenses",
             src=self.source_folder)

    def package_info(self):
        # DISABLE Conan's config file generation
        self.cpp_info.set_property("cmake_find_mode", "none")
        # Tell CMake to include this directory in its search path
        self.cpp_info.builddirs.append("lib/cmake")
