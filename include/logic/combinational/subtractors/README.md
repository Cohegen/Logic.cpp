# Binary Subtractors

The `logic::combinational::subtractors` module provides hardware circuits for performing binary subtraction operations using half-subtractors and full-subtractors with borrow propagation.

---

## Components

### 1. `HalfSubtractor`
Computes the binary difference between two 1-bit inputs ($A - B$), generating a 1-bit Difference ($D$) and a Borrow-Out ($B_{out}$).

* **Boolean Equations**:
  $$D = A \oplus B$$
  $$B_{out} = \bar{A} \cdot B$$

---

### 2. `FullSubtractor`
Computes the binary difference ($A - B - B_{in}$) taking into account an incoming Borrow-In ($B_{in}$).

* **Boolean Equations**:
  $$D = A \oplus B \oplus B_{in}$$
  $$B_{out} = (\bar{A} \cdot B) + (\overline{(A \oplus B)} \cdot B_{in})$$

---

## Truth Tables

### Full Subtractor Truth Table
| Input $A$ | Input $B$ | Borrow-In $B_{in}$ | Difference $D$ | Borrow-Out $B_{out}$ |
| :---: | :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 1 | 1 | 1 |
| 0 | 1 | 0 | 1 | 1 |
| 0 | 1 | 1 | 0 | 1 |
| 1 | 0 | 0 | 1 | 0 |
| 1 | 0 | 1 | 0 | 0 |
| 1 | 1 | 0 | 0 | 0 |
| 1 | 1 | 1 | 1 | 1 |

---

## Usage Example

```cpp
#include "combinational/subtractors/FullSubtractor.hpp"
#include "signals/wire.hpp"
#include <iostream>

int main() {
    logic::Wire a(logic::LogicState::LOW);
    logic::Wire b(logic::LogicState::HIGH);
    logic::Wire b_in(logic::LogicState::LOW);
    logic::Wire diff, b_out;

    logic::FullSubtractor fs(a, b, b_in, diff, b_out);
    fs.evaluate();

    std::cout << "0 - 1 Difference: " << static_cast<int>(diff.read()) << "\n"; // 1
    std::cout << "Borrow Out: " << static_cast<int>(b_out.read()) << "\n";       // 1

    return 0;
}
```
