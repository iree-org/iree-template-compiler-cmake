# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_llvm_executable(my-iree-opt my-iree-opt.cpp)

get_property(_MLIR_DIALECT_LIBS GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(_MLIR_CONVERSION_LIBS GLOBAL PROPERTY MLIR_CONVERSION_LIBS)

target_link_libraries(my-iree-opt PRIVATE
  MLIROptLib
  MyIREEInitAll
  MyIREETransforms
  ${_MLIR_DIALECT_LIBS}
  ${_MLIR_CONVERSION_LIBS}
)

install(
  TARGETS my-iree-opt
  EXPORT MyIREECompilerTargets
  RUNTIME DESTINATION ${LLVM_TOOLS_INSTALL_DIR}
  COMPONENT my-iree-opt
)
