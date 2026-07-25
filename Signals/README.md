# Signals Module

The `Signals` module provides the core primitives for representing logic states, signal values, physical wires, buses, clocks, and bit vectors in the simulation engine.

## Overview

In digital logic simulation, hardware connections carry multi-valued logic states rather than simple booleans. The `Signals` module implements IEEE 1164-style 4-valued logic (`0`, `1`, `X`, `Z`), wire connections, bus abstractions, timing clocks, and arbitrary-width bit manipulation.

---

## Core Components

| Header File | Class / Enum | Description |
|-------------|--------------|-------------|
| `logicstate.hpp` | `enum class LogicState` | Represents 4-valued digital logic states (`LOW`, `HIGH`, `UNKNOWN`, `HIGH_IMPEDANCE`) and provides logic operation functions. |
| `signal.hpp` | `class Signal` | Encapsulates a logic state carried on a net or connection. |
| `wire.hpp` | `class Wire` | Represents a physical conductor/wire used to connect logic components. |
| `bus.hpp` | `template <std::size_t N> class Bus` | Represents an $N$-bit bus (grouping of $N$ `Wire` objects). |
| `clock.hpp` | `class Clock` | Tracks cycle counts for driving sequential circuit evaluations. |
| `bitvector.hpp` | `template <std::size_t N> class BitVector` | Encapsulates an $N$-bit binary vector with bitwise operators and conversions. |

---

## Detailed Specifications

### 1. `LogicState` (`logicstate.hpp`)

Represents a 4-valued logic system:

- `LogicState::LOW` (`0`) — Logical 0 / False.
- `LogicState::HIGH` (`1`) — Logical 1 / True.
- `LogicState::UNKNOWN` (`X`) — Invalid or uninitialized state.
- `LogicState::HIGH_IMPEDANCE` (`Z`) — Tri-stated / floating wire connection.

#### Helper Functions & Conversions
- `to_char(LogicState state)` — Returns character representation (`'0'`, `'1'`, `'X'`, `'Z'`).
- `from_bool(bool val)` — Converts a boolean to `LogicState::HIGH` or `LogicState::LOW`.
- `is_binary(LogicState state)` — Checks if state is either `LOW` or `HIGH`.
- `to_bool(LogicState state)` — Converts binary state to boolean; throws `std::logic_error` if `UNKNOWN` or `HIGH_IMPEDANCE`.

#### 4-Valued Logic Operations
Evaluates standard logic gates over 4-valued logic tables:
- `logic_not(state)`
- `logic_and(state1, state2)`
- `logic_or(state1, state2)`
- `logic_xor(state1, state2)`
- `logic_nand(state1, state2)`
- `logic_nor(state1, state2)`
- `logic_xnor(state1, state2)`

---

### 2. `Signal` (`signal.hpp`)

Wraps a `LogicState` inside a signal container.

```cpp
namespace logic {
    Signal sig(LogicState::HIGH);
    LogicState val = sig.value(); // LogicState::HIGH
    sig.set(LogicState::LOW);
}
```

---

### 3. `Wire` (`wire.hpp`)

Models a physical wire connecting hardware components. Gates write states onto wires and read states from input wires.

```cpp
namespace logic {
    Wire w1(LogicState::LOW);
    w1.write(LogicState::HIGH);
    LogicState state = w1.read();
}
```

---

### 4. `Bus<N>` (`bus.hpp`)

Groups $N$ `Wire` instances together to represent multi-bit data, address, or control buses in hardware design.

```cpp
namespace logic {
    Bus<8> dataBus; // 8-bit data bus
    dataBus[0].write(LogicState::HIGH);
    dataBus.write(LogicState::LOW);     // Set all 8 wires to LOW
    dataBus.clear();                    // Reset all wires to LOW
}
```

Key features:
- `size()` — Returns bus width ($N$).
- `operator[]` & `at()` — Element access for individual wires.
- `front()` & `back()` — Access first and last wire.
- Iterators (`begin()`, `end()`, `cbegin()`, `cend()`) for modern C++ algorithms.

---

### 5. `Clock` (`clock.hpp`)

Tracks elapsed clock cycles for driving sequential circuits (flip-flops, registers, counters).

```cpp
Clock clk;
clk.tick(); // Increments cycle counter
auto currentCycle = clk.cycle();
```

---

### 6. `BitVector<N>` (`bitvector.hpp`)

Provides an $N$-bit binary vector wrapping `std::bitset<N>` with standard bitwise operators (`&`, `|`, `^`, `~`, `<<`, `>>`), compound assignments (`&=`, `|=`, `^=`, `<<=`, `>>=`), element mutation (`set`, `reset`, `flip`), inspection (`test`, `any`, `all`, `none`, `count`), and conversions (`to_string`, `to_ulong`, `to_ullong`).

```cpp
BitVector<4> bv("1010");
bv.flip(); // "0101"
```

---

## Design Flow

```
LogicState ('0', '1', 'X', 'Z')
      ↓ wrapped in
    Signal
      ↓ used by
     Wire
      ↓ grouped into
    Bus<N>
```
