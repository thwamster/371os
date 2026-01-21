# 371os

Repository for CS 371: Advanced Systems Computing in spring 2026 at Willamette University.

## Build Instructions

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
