// Copyright 2023 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// IREE compiler C API sample application.
//
// This is meant to show a minimal way to use IREE's compiler C API by
// running a compiler invocation on a sample program and printing the
// intermediate output.
//
// While the API is written in C for maximum portability, some C++ concepts can
// improve the ergonomics substantially - notably RAII types and string/list
// handling classes.
//
// Demo usage:
//   $ hello-compiler </path/to/libIREECompiler.so or IREECompiler.dll>

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <iree/compiler/embedding_api.h>
#include <iree/compiler/loader.h>

#define IREE_COMPILER_EXPECTED_API_MAJOR 1 // At most this major version
#define IREE_COMPILER_EXPECTED_API_MINOR 2 // At least this minor version

typedef struct compiler_state_t {
  iree_compiler_session_t *session;
  iree_compiler_source_t *source;
  iree_compiler_output_t *output;
  iree_compiler_invocation_t *inv;
} compiler_state_t;

void handle_compiler_error(iree_compiler_error_t *error) {
  const char *msg = ireeCompilerErrorGetMessage(error);
  fprintf(stderr, "Error from compiler API:\n%s\n", msg);
  ireeCompilerErrorDestroy(error);
}

void cleanup_compiler_state(compiler_state_t s) {
  if (s.inv)
    ireeCompilerInvocationDestroy(s.inv);
  if (s.output)
    ireeCompilerOutputDestroy(s.output);
  if (s.source)
    ireeCompilerSourceDestroy(s.source);
  if (s.session)
    ireeCompilerSessionDestroy(s.session);
  ireeCompilerGlobalShutdown();
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage:\n"
                    "  hello-compiler </path/to/libIREECompiler.so>\n");
    return -1;
  }

  // Load the compiler library then initialize it.
  // This should be done only once per process. If deferring the load or using
  // multiple threads, be sure to synchronize this, e.g. with std::call_once.
  bool result = ireeCompilerLoadLibrary(argv[1]);
  if (!result) {
    fprintf(stderr, "** Failed to initialize IREE Compiler **\n");
    return 1;
  }
  // Note: this must be balanced with a call to ireeCompilerGlobalShutdown().
  ireeCompilerGlobalInitialize();

  // To set global options (see `iree-compile --help` for possibilities), use
  // |ireeCompilerGetProcessCLArgs| and |ireeCompilerSetupGlobalCL| here.
  // For an example of how to splice flags between a wrapping application and
  // the IREE compiler, see the "ArgParser" class in iree-run-mlir-main.cc.

  // Check the API version before proceeding any further.
  uint32_t api_version = (uint32_t)ireeCompilerGetAPIVersion();
  uint16_t api_version_major = (uint16_t)((api_version >> 16) & 0xFFFFUL);
  uint16_t api_version_minor = (uint16_t)(api_version & 0xFFFFUL);
  fprintf(stdout, "Compiler API version: %" PRIu16 ".%" PRIu16 "\n",
          api_version_major, api_version_minor);
  if (api_version_major > IREE_COMPILER_EXPECTED_API_MAJOR ||
      api_version_minor < IREE_COMPILER_EXPECTED_API_MINOR) {
    fprintf(stderr,
            "Error: incompatible API version; built for version %" PRIu16
            ".%" PRIu16 " but loaded version %" PRIu16 ".%" PRIu16 "\n",
            IREE_COMPILER_EXPECTED_API_MAJOR, IREE_COMPILER_EXPECTED_API_MINOR,
            api_version_major, api_version_minor);
    ireeCompilerGlobalShutdown();
    return 1;
  }

  // Check for a build tag with release version information.
  const char *revision = ireeCompilerGetRevision();
  fprintf(stdout, "Compiler revision: '%s'\n", revision);

  // ------------------------------------------------------------------------ //
  // Initialization and version checking complete, ready to use the compiler. //
  // ------------------------------------------------------------------------ //

  compiler_state_t s;
  s.session = NULL;
  s.source = NULL;
  s.output = NULL;
  s.inv = NULL;

  iree_compiler_error_t *error = NULL;

  // A session represents a scope where one or more invocations can be executed.
  s.session = ireeCompilerSessionCreate();

  // Create a compiler 'source' by wrapping a string buffer.
  // A file could be opened instead with |ireeCompilerSourceOpenFile|.
  const char *simple_mul_mlir = " \
func.func @simple_mul(%lhs: tensor<4xf32>, %rhs: tensor<4xf32>) -> tensor<4xf32> {\n\
  %result = arith.mulf %lhs, %rhs : tensor<4xf32>\n \
  return %result : tensor<4xf32>\n \
}";
  error = ireeCompilerSourceWrapBuffer(s.session, "simple_mul", simple_mul_mlir,
                                       strlen(simple_mul_mlir) + 1,
                                       /*isNullTerminated=*/true, &s.source);
  if (error) {
    fprintf(stderr, "Error wrapping source buffer\n");
    handle_compiler_error(error);
    cleanup_compiler_state(s);
    return 1;
  }
  fprintf(stdout, "Wrapped simple_mul buffer as compiler source\n");

  // ------------------------------------------------------------------------ //
  // Inputs and outputs are prepared, ready to run an invocation pipeline.    //
  // ------------------------------------------------------------------------ //

  // Use an invocation to compile from the input source to the output stream.
  iree_compiler_invocation_t *inv = ireeCompilerInvocationCreate(s.session);
  ireeCompilerInvocationEnableConsoleDiagnostics(inv);

  if (!ireeCompilerInvocationParseSource(inv, s.source)) {
    fprintf(stderr, "Error parsing input source into invocation\n");
    cleanup_compiler_state(s);
    return 1;
  }

  // Compile up to the 'flow' dialect phase.
  // Typically a compiler tool would compile to 'end' and either output to a
  // .vmfb file for later usage in a deployed application or output to memory
  // for immediate usage in a JIT scenario.
  ireeCompilerInvocationSetCompileToPhase(inv, "flow");

  // Run the compiler invocation pipeline.
  if (!ireeCompilerInvocationPipeline(inv, IREE_COMPILER_PIPELINE_STD)) {
    fprintf(stderr, "Error running compiler invocation\n");
    cleanup_compiler_state(s);
    return 1;
  }
  fprintf(stdout, "Compilation successful, output:\n\n");

  // Create a compiler 'output' piped to the 'stdout' file descriptor.
  // A file or memory buffer could be opened instead using
  // |ireeCompilerOutputOpenFile| or |ireeCompilerOutputOpenMembuffer|.
  fflush(stdout);
  error = ireeCompilerOutputOpenFD(fileno(stdout), &s.output);
  if (error) {
    fprintf(stderr, "Error opening output file descriptor\n");
    handle_compiler_error(error);
    cleanup_compiler_state(s);
    return 1;
  }

  // Print IR to the output stream.
  // When compiling to the 'end' phase, a compiler tool would typically use
  // either |ireeCompilerInvocationOutputVMBytecode| or
  // |ireeCompilerInvocationOutputVMCSource|.
  error = ireeCompilerInvocationOutputIR(inv, s.output);
  if (error) {
    handle_compiler_error(error);
    cleanup_compiler_state(s);
    return 1;
  }

  cleanup_compiler_state(s);
  return 0;
}
