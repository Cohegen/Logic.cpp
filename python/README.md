# pylogic

Pythonic hardware simulation library powered by the high-performance C++20 `Logic.cpp` engine.

## Features
- **PyTorch-like Operator Overloading**: Combine wires and buses using `&`, `|`, `^`, `~`, `+`.
- **4-State Logic**: Full fidelity hardware modeling (`LOW`, `HIGH`, `UNKNOWN`, `HIGH_IMPEDANCE`).
- **Flexible Bus Slicing**: Slice buses just like Python lists/arrays (`bus[0:8]`).
- **C++20 Speed**: Backed by pre-compiled C++ simulation kernels via `pybind11`.

## Installation & Build
```bash
# Using CMake:
cmake -B build -S . -G "MinGW Makefiles" -DPYTHON_EXECUTABLE=python
cmake --build build

# Or via pip:
pip install .
```

## Quick Start
```python
import pylogic
from pylogic import Wire, Bus, HalfAdder

# Wires with operator overloading
a = Wire(1)
b = Wire(0)
c = a & b
d = a ^ b
print(c.value)  # 0
print(d.value)  # 1

# Hardware Bus and bit slicing
bus = Bus(32, value=0xDEADBEEF)
lower_byte = bus[0:8]
print(hex(lower_byte.value))  # 0xEF

# 32-bit hardware ripple-carry addition
bus_a = Bus(32, 120)
bus_b = Bus(32, 80)
sum_bus = bus_a + bus_b
print(sum_bus.value)  # 200

# Structural components
adder = HalfAdder(a, b)
sum_out, carry_out = adder()
```
