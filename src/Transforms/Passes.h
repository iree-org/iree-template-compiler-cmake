// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef MYIREECOMPILER_TRANSFORMS_PASSES_H_
#define MYIREECOMPILER_TRANSFORMS_PASSES_H_

#include "iree/compiler/Dialect/HAL/IR/HALTypes.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"

namespace mlir {
class ModuleOp;

namespace torch {
namespace RefBackend {

/// Registers all RefBackend passes.
void registerRefBackendPasses();

std::unique_ptr<OperationPass<ModuleOp>> createMungeCallingConventionsPass();

}  // namespace RefBackend
}  // namespace torch
}  // namespace mlir

#endif  // MYIREECOMPILER_TRANSFORMS_PASSES_H_
