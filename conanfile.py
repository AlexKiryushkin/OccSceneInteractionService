from conan import ConanFile
import os

class ConanDependencies(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    requires = (
        "opencascade/7.6.2",
        "gtest/1.15.0",
    )

    generators = (
        "CMakeDeps",
        "CMakeToolchain",
    )
    
    def requirements(self):
        qt_env_var_name = "QT_DIR"
        if qt_env_var_name not in os.environ:
            self.requires("qt/6.6.1")

        self.requires("freetype/2.13.0", override=True)
        self.requires("libpng/1.6.42", override=True)
