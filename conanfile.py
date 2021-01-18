from conans import ConanFile, CMake, tools


class BaldrConan(ConanFile):
    name = "baldr"
    requires = (("eigen/3.3.7@conan/stable"),
                ("fmt/6.0.0@bincrafters/stable"),
                ("glfw/3.2.1@bincrafters/stable", "private"),
                ("glew/2.1.0@bincrafters/stable"))
    version = "0.1"
    license = "unlicense"
    author = "Richard Vock vock@cs.uni-bonn.de"
    url = "https://github.com/richard-vock/baldr"
    description = "Modern OpenGL wrapper library"
    topics = ("opengl")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "verbose": [True, False]}
    default_options = "shared=False", "verbose=False"
    generators = "cmake"
    exports_sources = "include*", "src*", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)
        cmake.definitions["DEBUG_VERBOSE"] = self.options.verbose
        cmake.configure()#source_folder="hello")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("include/*.hpp")
        self.copy("include/*.ipp")
        self.copy("*baldr.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["baldr"]

