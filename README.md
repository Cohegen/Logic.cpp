# Logic.cpp
Implementation of concepts in Digital Design using C++ Language.

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
