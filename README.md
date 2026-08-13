# Logic.cpp

![out:](assets/logo2.png)

Logic.cpp is a C++20 hardware-simulation library for building digital logic systems from small, reusable components. It models signals, wires, gates, combinational circuits, sequential circuits, memory blocks, and simulation utilities so larger computer architecture projects can be assembled from the same lower-level building blocks used in digital design.

The goal of Logic.cpp is to provide a practical foundation for experimenting with hardware concepts in C++. Instead of treating circuits as ordinary software calculations, the library is designed around explicit hardware-style components such as gates, adders, subtractors, multiplexers, latches, flip-flops, registers, counters, buses, RAM, ROM, and control units. These components can then be composed into larger systems, including external projects such as CPU simulators.

## Project Goals

- Model digital hardware concepts using clear C++ components.
- Make combinational and sequential circuits reusable across projects.
- Support CPU and computer architecture experiments without duplicating logic components.
- Keep the design close to hardware structure, where larger modules are built from smaller modules.
- Provide examples and tests that show how each component behaves.

## Current Scope

Logic.cpp currently includes core signal types, gates, arithmetic components, bitwise units, multiplexers, latches, flip-flops, registers, counters, memory components, and simulator infrastructure. The project is still under active development, so some APIs and examples may change as the component library grows.

## Build

This project uses CMake.

Prerequisites:

- CMake 3.20 or newer
- A C++ compiler toolchain such as MSVC Build Tools, MinGW-w64, or Clang

```powershell
cmake --preset default
cmake --build --preset default
```

## Run

On Windows CMake generators such as Visual Studio usually place the executable under `build\Debug`:

```powershell
.\build\Debug\logic_cpp.exe
```

Single-configuration generators place it directly under `build`:

```powershell
.\build\logic_cpp.exe
```

## Note

Work is still underway, so this project is not complete.
