// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYIREECOMPILER_TRANSFORMS_PASSES
#define MYIREECOMPILER_TRANSFORMS_PASSES

include "mlir/Pass/PassBase.td"

def XXX : Pass<"my-xxx", "ModuleOp"> {
  let summary = "Munge calling conventions for calling via ExecutionEngine";
  let constructor = "mlir::my::createXXXPass();";
  let dependentDialects = [
    "IREE::HAL::HALDialect"
  ];
}

#endif // MYIREECOMPILER_TRANSFORMS_PASSES
