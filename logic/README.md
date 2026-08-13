# Logic.cpp Core Library Overview

`Logic.cpp` is a C++20 digital logic simulation framework designed for hardware modeling, computer architecture experiments, and digital design prototyping. It enables building complex digital systems starting from primitive signals and logic gates up to complete ALUs, memory hierarchies, and CPU components.

---

## Directory Structure & Subsystem Overview

The library is organized into modular subsystems under the `logic/` namespace:

```
logic/
├── signals/        # Wires, Buses, Clocks, LogicState enums, and BitVectors
├── gates/          # Primitive logic gates (AND, OR, NOT, NAND, NOR, XOR, XNOR, BUFFER)
├── combinational/  # Purely combinational circuit components
│   ├── adders/       # HalfAdder, FullAdder, RippleCarryAdder
│   ├── subtractors/  # HalfSubtractor, FullSubtractor
│   ├── alu/          # Configurable Arithmetic Logic Unit
│   ├── multiplexers/ # Mux (2:1 to N:1) and Demux (1:2 to 1:N)
│   ├── decoders/     # Decoders (2:4, 3:8) and Priority Encoders
│   ├── comparators/  # Bit and N-bit Magnitude Comparators
│   └── bitwise/      # Bus-level bitwise operation units
├── sequential/     # Clocked and stateful components
│   ├── latches/      # SR, Gated-SR, D-Latches
│   ├── flipflops/    # D, SR, JK, T Flip-Flops
│   ├── registers/    # Parallel Registers, Shift Registers (SISO, SIPO, PISO, PIPO)
│   ├── counters/     # Ripple, Binary, Up/Down, Modulo, Ring, Johnson Counters
│   └── memory/       # RegisterFile, ROM, RAM, MemoryController, Integrated Memory
└── simulator/      # Event-driven and cycle-based execution framework
```

---

## Key Hardware Abstractions

All digital components inherit from `logic::Component` or specialized base classes:

| Class | Description |
| :--- | :--- |
| `logic::LogicState` | 4-state logic enum (`LOW`, `HIGH`, `UNDEFINED`, `HIGH_IMPEDANCE`). |
| `logic::Wire` | 1-bit signal connection carrying `LogicState`. |
| `logic::Bus<N>` | Templated $N$-bit bus collection of `Wire` references or objects. |
| `logic::Clock` | Square-wave clock signal generator with rising and falling edge triggers. |
| `logic::Component` | Abstract base class enforcing `evaluate() noexcept` contract for all circuits. |

---

## Integration in External Projects

`Logic.cpp` is configured as a CMake target `logic_cpp`. You can include it in sibling projects (such as `CPU.cpp`) via standard CMake target linking:

```cmake
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Logic.cpp ${CMAKE_CURRENT_BINARY_DIR}/logic_build)

add_executable(my_cpu main.cpp)
target_link_libraries(my_cpu PRIVATE logic_cpp)
target_include_directories(my_cpu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../Logic.cpp)
```

---

## C++ Quick Start Example

```cpp
#include "logic/signals/wire.hpp"
#include "logic/gates/AND.hpp"
#include "logic/gates/OR.hpp"
#include <iostream>

int main() {
    logic::Wire input_a, input_b, out_and, out_or;
    logic::ANDGate and_gate(input_a, input_b, out_and);
    logic::ORGate  or_gate(input_a, input_b, out_or);

    // Set inputs A=1, B=0
    input_a.write(logic::LogicState::HIGH);
    input_b.write(logic::LogicState::LOW);

    // Evaluate component logic
    and_gate.evaluate();
    or_gate.evaluate();

    std::cout << "AND Output: " << out_and.read() << "\n"; // Output: LOW (0)
    std::cout << "OR Output:  " << out_or.read()  << "\n"; // Output: HIGH (1)

    return 0;
}
```
