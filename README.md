# 371os

Repository for CS 371: Advanced Systems Computing in spring 2026 at Willamette University.

## Build Instructions

This repository includes both a CMake workspace in C and a Cargo workspace in Rust at the root directory. Both
workspaces use the `/build` directory.

### CMake

Requires a `riscv64-unknown-elf-gcc` toolchain, which can be downloaded prebuilt
from [here](https://github.com/aitesam961/riscv-gnu-prebuilt.git) or compiled from
source [here](https://github.com/riscv-collab/riscv-gnu-toolchain.git). Additionally, requires CMake itself,
`qemu-system-riscv64`, and Ninja.

To build with CMake:

```console
mkdir build
cd build
cmake ..
make run_<PROJECT_NUMBER>
cd ..
```

To test with CMake:

```console
mkdir build
cd build
cmake ../
make test_<PROJECT_NUMBER>
cd ..
ctest -V --test-dir build -R "<PROJECT_NUMBER>"
```

Some of the CMake host projects require command line arguments which are currently not supported. To run these:

```console
mkdir build
cd build
cmake ..
make
cd host/<PROJECT_NUMBER>
./<PROJECT_NAME>
cd ../..
```

### Cargo

Requires only Cargo itself. To build with Cargo:

```console
cargo run --bin <PROJECT_NAME>
```

Some of the Cargo projects require building inside of their directory. If the above throws an error:

```console
cd rs/<PROJECT_NUMBER>
cargo run
cd ../..
```
