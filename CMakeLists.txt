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
# IREE is a toolkit and has many optional features and build tuning parameters
# that will differ for everyone integrating it. This extends from big knobs
# like which HAL backends to support (CUDA, Vulkan, CPU single-threaded,
# CPU multi-threaded, etc) and to small knobs like whether to preserve
# iree_status_t failure messages (and the associated string manipulation code)
# or treat them like integers for code size reduction.

# Disable building the IREE compiler and test files.
# Compiler builds pull in the LLVM build and make cross-compiling difficult.
# It's recommended that the IREE packages are installed from pip or the compiler
# is built from source from the main IREE repository and installed for use by
# the subprojects involving the runtime. The main IREE repository compiles
# models as part of its build process but for most user workflows compilation
# can be handled much better by scripts and frontend integrations.
set(IREE_BUILD_COMPILER ON)
set(IREE_BUILD_TESTS OFF)
set(IREE_BUILD_SAMPLES OFF)

# Include the IREE submodule CMake project as a dependency.
# This will make the IREE targets available for use by subsequent commands.
#
# Note that EXCLUDE_FROM_ALL will prevent the IREE targets from showing in the
# CMake target listing by default - for most users this is fine but if deeply
# integrating it can be useful to drop it.
add_subdirectory(third_party/iree EXCLUDE_FROM_ALL)
