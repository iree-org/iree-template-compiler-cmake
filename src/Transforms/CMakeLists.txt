# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(LLVM_TARGET_DEFINITIONS Passes.td)
mlir_tablegen(Passes.h.inc -gen-pass-decls)
add_public_tablegen_target(MyIREETransformsPassIncGen)

add_mlir_library(MyIREETransforms
  Passes.cpp

  # ADDITIONAL_HEADER_DIRS
    # ${PROJECT_SRC_DIR}/include/torch-mlir/RefBackend

  DEPENDS
    MyIREETransformsPassIncGen

  LINK_COMPONENTS
    Core

  LINK_LIBS PUBLIC
    MLIRIR
    MLIRTransforms
    MLIRMathTransforms
)

mlir_check_all_link_libraries(MyIREETransforms)
