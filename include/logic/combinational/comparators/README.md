# Binary Comparators

The `logic::combinational::comparators` module provides magnitude comparison components for single-bit and multi-bit data buses.

---

## Components

### 1. `BitComparator`
Compares two 1-bit binary signals ($A, B$) and evaluates three relational flags:
* $A > B$ (`greater`)
* $A = B$ (`equal`)
* $A < B$ (`less`)

### 2. `MagnitudeComparator<Width>`
Compares two unsigned $N$-bit binary buses (`Bus<Width> A`, `Bus<Width> B`) starting from the most significant bit (MSB) down to the least significant bit (LSB).

---

## Output Signal Interface

| Output Wire | Boolean Equation / Condition |
| :--- | :--- |
| `greater` | Evaluates `HIGH` if unsigned $A > B$. |
| `equal` | Evaluates `HIGH` if $A == B$ across all bits. |
| `less` | Evaluates `HIGH` if unsigned $A < B$. |

---

## Usage Example

```cpp
#include "combinational/comparators/MagnitudeComparator.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include <iostream>

int main() {
    logic::Bus<8> bus_a, bus_b;
    logic::Wire is_greater, is_equal, is_less;

    logic::MagnitudeComparator<8> comp(bus_a, bus_b, is_greater, is_equal, is_less);

    bus_a.write_value(42);
    bus_b.write_value(100);

    comp.evaluate();

    std::cout << "42 > 100: " << static_cast<int>(is_greater.read()) << "\n"; // 0
    std::cout << "42 == 100: " << static_cast<int>(is_equal.read())   << "\n"; // 0
    std::cout << "42 < 100: "  << static_cast<int>(is_less.read())    << "\n"; // 1

    return 0;
}
```
