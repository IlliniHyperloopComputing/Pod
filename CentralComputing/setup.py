#! /usr/bin/env python
from distutils.core import setup
from distutils.extension import Extension

setup(name="PackageName",
    ext_modules=[
        Extension("commandqueue", ["Command.cpp"],
        libraries = ["boost_python"], extra_compile_args=['-std=c++11'])
    ])
