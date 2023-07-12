# IREE Compiler Hello World with CMake

![Build with Latest IREE Release](https://github.com/iree-org/iree-template-compiler-cmake/workflows/IREE%20Compiler%20Template/badge.svg)

## Instructions

### Cloning the Repository

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
