# Signals Infrastructure

The `logic::signals` module defines the foundational primitives representing electrical states, physical wires, multi-bit buses, bit vectors, and square-wave clock sources across digital hardware components.

---

## Component Class Hierarchy

```
            +--------------------+
            |   LogicState Enum  | (LOW, HIGH, UNDEFINED, HIGH_IMPEDANCE)
            +---------+----------+
                      |
        +-------------+-------------+
        |                           |
        v                           v
  +-----------+               +-----------+
  |   Wire    |               | BitVector |
  +-----+-----+               +-----------+
        |
        v
  +-----------+
  |  Bus<N>   |
  +-----------+
        |
        v
  +-----------+
  |   Clock   | (Specialized Wire with toggle capabilities)
  +-----------+
```

---

## 1. `LogicState`

Represents digital voltage logic states in four-valued logic system:

| State Enum | Value | Hardware Equivalent |
| :--- | :--- | :--- |
| `LogicState::LOW` | `0` | Logic 0 / Ground ($0\text{V}$) |
| `LogicState::HIGH` | `1` | Logic 1 / $V_{DD}$ ($3.3\text{V}$ / $5\text{V}$) |
| `LogicState::UNDEFINED` | `X` | Uninitialized / Floating / Contention |
| `LogicState::HIGH_IMPEDANCE` | `Z` | Tri-State High Impedance (Disconnected) |

---

## 2. `Wire`

A `Wire` represents a single physical connection carrying a `LogicState`. Wires can have listeners attached to detect signal value changes automatically during simulation.

### C++ Signature
```cpp
class Wire {
public:
    Wire(LogicState state = LogicState::LOW) noexcept;
    
    void write(LogicState state) noexcept;
    [[nodiscard]] LogicState read() const noexcept;
    
    // Explicit boolean conversion (HIGH -> true, LOW/UNDEFINED -> false)
    explicit operator bool() const noexcept;
};
```

---

## 3. `Bus<N>`

`Bus<N>` represents a fixed-width bundle of $N$ `Wire` connections (e.g. 8-bit, 16-bit, 32-bit data/address buses).

### C++ Signature
```cpp
template <std::size_t Width>
class Bus {
public:
    Bus() noexcept;
    
    // Access individual wire at index bit
    Wire& operator[](std::size_t index);
    const Wire& operator[](std::size_t index) const;
    
    // Integer conversion utilities
    void write_value(std::uint64_t value) noexcept;
    [[nodiscard]] std::uint64_t read_value() const noexcept;
};
```

---

## 4. `Clock`

Specialized signal component providing square-wave clock pulse generation for synchronous sequential logic.

### Usage
```cpp
logic::Clock clock;

clock.tick(); // Transitions LOW -> HIGH (Rising Edge)
clock.tick(); // Transitions HIGH -> LOW (Falling Edge)
```

---

## Usage Example

```cpp
#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include <iostream>

int main() {
    logic::Clock clock;
    logic::Bus<8> data_bus;

    // Write 0xA5 (10100101 in binary) to 8-bit bus
    data_bus.write_value(0xA5);

    std::cout << "Bus Value: 0x" << std::hex << data_bus.read_value() << "\n";
    std::cout << "Bit 0 State: " << static_cast<int>(data_bus[0].read()) << "\n";

    return 0;
}
```
