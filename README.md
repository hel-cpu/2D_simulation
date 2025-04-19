# Project Name
A simple 2D simulation of a Ball

# How to Build
This project uses [vcpkg](https://github.com/microsoft/vcpkg.git) for dependency management. <br>
Built with CMake with the vcpkg toolchain

## Prerequisites

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/) (≥ 3.10)
- C++20 compatible compiler
- [vcpkg](https://vcpkg.io/)

## Build

`vcpkg new --application` <br>
`vcpkg integrate install` <br>
`cmake -B build -DCMAKE_TOOLCHAIN_FILE="[vcpkg-root]/scripts/buildsystems/vcpkg.cmake"` <br>