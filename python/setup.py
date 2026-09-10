import os
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "pylogic._pylogic_core",
        ["src/bindings.cpp"],
        include_dirs=[
            os.path.abspath("../include"),
            os.path.abspath("../include/logic"),
        ],
        cxx_std=20,
    ),
]

setup(
    name="pylogic",
    version="0.1.0",
    packages=find_packages(),
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
