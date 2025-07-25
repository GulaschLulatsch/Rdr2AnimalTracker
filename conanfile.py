from conan import ConanFile
from conan.tools.cmake import cmake_layout

class AnimalTracker(ConanFile):
    name = "AnimalTracker"
    license = "MIT License"
    languages = "C", "C++"
    generators = "CMakeDeps", "CMakeToolchain", "VirtualBuildEnv"
    settings = "os", "arch", "compiler", "build_type"

    def requirements(self):
        self.requires("spdlog/1.15.3")
        self.requires("icu/77.1")

    def build_requirements(self):
        self.tool_requires("cmake/4.0.3")

    # The purpose of generate() is to prepare the build, generating the necessary files, such as
    # Files containing information to locate the dependencies, environment activation scripts,
    # and specific build system files among others
    def generate(self):
        pass

    # This method is used to build the source code of the recipe using the desired commands.
    def build(self):
        # You can use your command line tools to invoke your build system
        # or any of the build helpers provided with Conan in conan.tools
        # self.run("g++ ...")
        pass

    # The actual creation of the package, once it's built, is done in the package() method.
    # Using the copy() method from tools.files, artifacts are copied
    # from the build folder to the package folder
    def package(self):
        # copy(self, "*.h", self.source_folder, join(self.package_folder, "include"), keep_path=False)
        pass
        
    def layout(self):
        cmake_layout(self)
