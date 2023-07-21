# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# IREE requires a minimum CMake version of 3.21 but may be possible to get
# working with older versions.
cmake_minimum_required(VERSION 3.21...3.24)

#-------------------------------------------------------------------------------
# Project configuration
#-------------------------------------------------------------------------------

project(iree-template-compiler-cmake VERSION 1.0 LANGUAGES CXX C)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-------------------------------------------------------------------------------
# IREE subproject configuration
#-------------------------------------------------------------------------------

# Enable building the IREE compiler.
#
# Projects depending on the compiler need three main components:
#   1. Compiler API header files from `iree/compiler/bindings/c/iree/compiler/`
#   2. The compiler shared library loader
#       - From CMake, link against `iree_compiler_bindings_c_loader`
#       - Or build `iree/compiler/bindings/c/iree/compiler/loader/` using the
#         toolchain of your choosing (no dependencies, reasonable to just fork)
#   3. The compiler shared library (libIREECompiler.so / IREECompiler.dll)
#       - This can be built from source with CMake
#       - This can also be installed from a release (such as a Python wheel)
set(IREE_BUILD_COMPILER ON)

# Disable optional IREE components.
set(IREE_BUILD_TESTS OFF)
set(IREE_BUILD_SAMPLES OFF)

# Include the IREE submodule CMake project as a dependency.
# This will make the IREE targets available for use by subsequent commands.
#
# Note that EXCLUDE_FROM_ALL will prevent the IREE targets from showing in the
# CMake target listing by default - for most users this is fine but if deeply
# integrating it can be useful to drop it.
add_subdirectory(third_party/iree EXCLUDE_FROM_ALL)

#-------------------------------------------------------------------------------
# Application
#-------------------------------------------------------------------------------

add_subdirectory(hello_compiler)
