# Urania

## Overview

**`Urania` is a physics engine written in C89 with the intent to explore various solvers for astrophysics, thermodynamic, and statisitcal mechanics systems.** It's designed to be available for cross-platform compilation and with minimal dependencies.


## Getting the engine

### Binary downloads

Official binaries and releases for the engine can be found in the [GitHub repository's releases](https://github.com/Anthony-J-G/urania-physx/releases). 

### Compiling from source using Zig

The engine utilizes the [Zig programming language](https://ziglang.org/) and it's built-in buildsystem as the preferred way of compiling the source code. The buildsystem comes bundled with it's own C/C++ compiler to make compiling the program extremely straightforward. The `build.zig` script aims to be always compatible with the latest [Zig release version](https://github.com/ziglang/zig/releases/tag/0.14.1).

The following command wll compile a release version of the engine binary for the host system and place it in the path `./zig-out/lib` when run from the root of the repository (`./zig-out/bin` if compiling as a dynamic library):

```bash
zig build -Dengine -Doptimze=ReleaseSafe
```

Another feature of Zig's buildsystem is the ability to cross-compile from one host platform to another. The target host can be specified by adding the `-Dtarget` flag, for example:

```bash
zig build -Dengine -Dtarget=x86_64-windows -Doptimze=ReleaseSafe
```

For the full set of build options, you can run `zig build -h` from the command line to view them all.

### Other compilation methods

For compatability reasons, the repository also features a [`CMakeLists.txt`](/CMakeLists.txt) for those who prefer to use CMake. Only compilation of the engine binary (statically or dynamically) is supported using this method.

Even though the engine uses a buildsystem for the compilation process, building the source files via an unofficial buildsystem or manually can be accomplished rather easily.


## Tests and examples

Included in the repository is a sample executable `examples` that contains an implementation of an editor and renderer that uses the engine to render various test scenes. Even though the engine has no dependencies, the `examples` executable requires both Raylib and Dear ImGui. Both of these dependencies are installed automatically via the Zig buildsystem.

Building of the examples is unsupported via CMake.


## Documentation

The official documentation can be found [here](https://docs.godotengine.org), hosted via GitHub Pages. Additionally, the source files for the docs can be found in the [`docs/`](/docs) folder of the repository as well as in comments of the various source and header files.

### Building the documentation
A large portion of the documentation is generated automatically using Doxygen and it's [configuration file](/docs/Doxyfile). Additionally, documentation content is compiled using a variety of Python tools such as Sphinx which can installed using `pip` and the included [requirements.txt](/docs/requirements.txt) file.

