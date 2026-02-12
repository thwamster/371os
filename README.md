# 371os

Repository for CS 371: Advanced Systems Computing in spring 2026 at Willamette University.

## Build Instructions

### C Binaries

Requires CMake. On UNIX architecture:

```console
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
```

On Windows:

```console
mkdir build
cd build
cmake -G "Visual Studio 18 2026" -A x64 ..
cmake --build .
cd Debug
```

Substitute your preferred generator and adjust accordingly.

### Rust Binaries

#### 32 [osirs]

Requires Qemu, particularly `qemu-system-misc`.

```console
rustc -C link-args=-Tmemory.x -C link-args=-Tlink.x -C panic=abort --target riscv64imac-unknown-none-elf src/main.rs
qemu-system-riscv64 -kernel main -bios none -machine virt -nographic
```

`Ctrl` + `A` then `H` for help.
