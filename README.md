# IREE Compiler Hello World with CMake

![Build with Latest IREE Release](https://github.com/iree-org/iree-template-compiler-cmake/workflows/IREE%20Compiler%20Template/badge.svg)

## Instructions

### Cloning the repository

Use GitHub's "Use this template" feature to create a new repository or clone it
manually:

```sh
$ git clone https://github.com/iree-org/iree-template-compiler-cmake.git
$ cd iree-template-compiler-cmake
$ git submodule update --init --recursive
```

The only requirement is that the main IREE repository is added as a submodule.
If working in an existing repository then add the submodule and ensure it has
its submodules initialized:

```sh
$ git submodule add https://github.com/openxla/iree.git third_party/iree/
$ git submodule update --init --recursive
```

### Acquire `libIREECompiler.so`

#### Option 1: Build the compiler from source

This lets you use static linking rather than dynamic linking or choose how the
compiler is composed (which optional components are enabled, using toolchain
features like sanitizers, etc.).

```shell
$ cmake -B build/ -G Ninja .
$ cmake --build build/ --target iree-compile

# Find the compiler shared object that the compiler binary uses.
$ ldd build/third_party/iree/tools/iree-compile
$ ls build/third_party/iree/lib/
libIREECompiler.so  libIREECompiler.so.0
```

#### Option 2: Installing the compiler from a binary distribution

Refer to the IREE
[Python bindings documentation](https://openxla.github.io/iree/reference/bindings/python/)
for more details on PIP packages.

```shell
# Create a virtual environment to download packages into.
$ python -m venv .venv
$ source .venv/bin/activate

# Install the iree-compiler package from a release (stable or nightly).
$ python -m pip install iree-compiler

# Find where the compiler was installed to.
# E.g. `./.venv/lib/python3.11/site-packages/iree/compiler/`
$ COMPILER_PATH=$(python -c "import iree.compiler as _; print(_.__path__[0])")

# Find the compiler shared object.
$ ls $COMPILER_PATH/_mlir_libs/
iree-compile  libIREECompiler.so
```

### Running the sample

```shell
# Build with CMake.
$ cmake -B build/ -G Ninja .
$ cmake --build build/ --target hello-compiler

# Run the sample.
$ ./build/hello_compiler/hello-compiler </path/to/libIREECompiler.so>
```
