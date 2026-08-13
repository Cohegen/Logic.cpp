# Logic Gates Subsystem

The `logic::gates` module contains implementations of basic single-bit digital logic primitives. These gates inherit from `logic::Gate` or `logic::BinaryGate` and process `logic::Wire` inputs to output deterministically evaluated states onto an output `logic::Wire`.

---

## Implemented Logic Gates

| Component | Truth Table / Function | Inputs | Output | Description |
| :--- | :--- | :--- | :--- | :--- |
| `ANDGate` | $Y = A \cdot B$ | 2 Wires ($A, B$) | 1 Wire ($Y$) | Outputs `HIGH` only when both inputs are `HIGH`. |
| `ORGate` | $Y = A + B$ | 2 Wires ($A, B$) | 1 Wire ($Y$) | Outputs `HIGH` if at least one input is `HIGH`. |
| `NOTGate` | $Y = \bar{A}$ | 1 Wire ($A$) | 1 Wire ($Y$) | Inverts input signal (`HIGH` $\leftrightarrow$ `LOW`). |
| `NANDGate` | $Y = \overline{A \cdot B}$ | 2 Wires ($A, B$) | 1 Wire ($Y$) | Universal gate; inverse of `AND`. |
| `NORGate` | $Y = \overline{A + B}$ | 2 Wires ($A, B$) | 1 Wire ($Y$) | Universal gate; inverse of `OR`. |
| `XORGate` | $Y = A \oplus B$ | 2 Wires ($A, B$) | 1 Wire ($Y$) | Exclusive-OR; outputs `HIGH` when inputs differ. |
| `XNORGate` | $Y = \overline{A \oplus B}$ | 2 Wires ($A, B$) | 1 Wire ($Y$) | Exclusive-NOR; outputs `HIGH` when inputs are equal. |
| `BufferGate` | $Y = A$ | 1 Wire ($A$) | 1 Wire ($Y$) | Non-inverting signal propagation / driver buffer. |

---

## Gate Architecture & Class Hierarchy

```
                    +--------------------+
                    |  logic::Component  |
                    +---------+----------+
                              |
                     +--------+--------+
                     |   logic::Gate   |
                     +--------+--------+
                              |
             +----------------+----------------+
             |                                 |
             v                                 v
   +--------------------+            +-------------------+
   | logic::BinaryGate  |            |   logic::NOTGate  |
   +---------+----------+            | logic::BufferGate |
             |                       +-------------------+
  +----------+----------+
  |  ANDGate |  ORGate  |
  | NANDGate | NORGate  |
  |  XORGate | XNORGate |
  +---------------------+
```

---

## Signal Handling Rules

- If an input wire is `LogicState::UNDEFINED` or `LogicState::HIGH_IMPEDANCE`, gate evaluation propagates `LogicState::UNDEFINED` unless bound by deterministic logic (e.g. `0 AND X = 0`).
- Gate evaluation occurs via `.evaluate() noexcept`.

---

## Usage Example

```cpp
#include "gates/AND.hpp"
#include "gates/NOT.hpp"
#include "signals/wire.hpp"
#include <iostream>

int main() {
    logic::Wire in_a(logic::LogicState::HIGH);
    logic::Wire in_b(logic::LogicState::HIGH);
    logic::Wire and_out, not_out;

    logic::ANDGate and_gate(in_a, in_b, and_out);
    logic::NOTGate not_gate(and_out, not_out);

    and_gate.evaluate();
    not_gate.evaluate();

    std::cout << "AND Result: " << static_cast<int>(and_out.read()) << "\n"; // 1
    std::cout << "NAND Cascade Result: " << static_cast<int>(not_out.read()) << "\n"; // 0

    return 0;
}
```
