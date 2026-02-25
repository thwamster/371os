# 371os

Repository for CS 371: Advanced Systems Computing in spring 2026 at Willamette University.

## Build Instructions

This repository includes both a CMake workspace in C and a Cargo workspace in Rust at the root directory. Both
workspaces
use the `/build` directory.

### CMake

To build with CMake:

```console
mkdir build
cd build
cmake ..
make run_<PROJECT_NUMBER>
```

Some of the CMake host projects require command line arguments which are currently not supported. To run these:

```
mkdir build
cd build
cmake ..
make
cd host/<PROJECT_NUMBER>
./<PROJECT_NAME
```

### Cargo

To build with Cargo:

```
cargo run --bin <PROJECT_NAME>
```

Some of the Cargo projects require building inside of their directory. If the above throws an error:

```
cd rs/<PROJECT_NUMBER>
cargo run
```