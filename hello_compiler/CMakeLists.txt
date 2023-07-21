# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(_NAME "hello-compiler")
add_executable(${_NAME} "hello_compiler.c")

# Include C API headers as system libraries.
# The headers could also be forked into application code (just be sure to
# check that the API version matches what you expect before calling functions).
set(_COMPILER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/iree/compiler")
target_include_directories(${_NAME} SYSTEM
  PRIVATE
    ${_COMPILER_ROOT}/bindings/c/iree/compiler
)

# Link in the IREE compiler API loader.
# This is a small target with no indirect dependencies that just handles finding
# the shared library and loading its symbols, so it can either be used like this
# or it can be forked into application code.
target_link_libraries(${_NAME} iree_compiler_bindings_c_loader)
