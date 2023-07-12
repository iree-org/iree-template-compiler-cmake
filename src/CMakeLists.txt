# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_subdirectory(Transforms)

add_mlir_library(MyIREEInitAll
  InitAll.cpp

  LINK_COMPONENTS
    Core

  LINK_LIBS PUBLIC
    MLIRFuncDialect
    MLIRIR
    MLIRSupport
    MyIREETransforms
)
