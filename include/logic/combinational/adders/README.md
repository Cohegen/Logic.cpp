# Binary Adders

The `logic::combinational::adders` package provides modular binary addition hardware components, ranging from 1-bit primitives to multi-bit ripple-carry adders.

---

## Components

### 1. `HalfAdder`
Adds two 1-bit binary inputs ($A, B$) producing a 1-bit Sum ($S$) and 1-bit Carry-Out ($C_{out}$).

* **Boolean Equations**:
  $$S = A \oplus B$$
  $$C_{out} = A \cdot B$$

### 2. `FullAdder`
Adds two 1-bit binary inputs ($A, B$) and a 1-bit Carry-In ($C_{in}$) producing a Sum ($S$) and Carry-Out ($C_{out}$).

* **Boolean Equations**:
  $$S = A \oplus B \oplus C_{in}$$
  $$C_{out} = (A \cdot B) + (C_{in} \cdot (A \oplus B))$$

### 3. `RippleCarryAdder<Bits>`
N-bit parallel binary adder constructed by chaining $N$ `FullAdder` stages in series where $C_{out, i}$ connects to $C_{in, i+1}$.

---

## Signal Interface & Architecture Diagram

```
                 Full Adder (1-Bit Stage)
               +---------------------------+
       A ----->|                           |
       B ----->|         FullAdder         |-----> Sum (S)
  C_in ----->|                           |-----> C_out
               +---------------------------+


          Ripple Carry Adder (4-Bit Cascaded Chain)
          
        +---------+     +---------+     +---------+     +---------+
 A[0] ->| Full    |  +->| Full    |  +->| Full    |  +->| Full    |
 B[0] ->| Adder 0 |  |  | Adder 1 |  |  | Adder 2 |  |  | Adder 3 |
C_in -->|         |--+  |         |--+  |         |--+  |         |---> C_out
        +----+----+     +----+----+     +----+----+     +----+----+
             |               |               |               |
             v               v               v               v
            S[0]            S[1]            S[2]            S[3]
```

---

## Usage Example

```cpp
#include "combinational/adders/RippleCarryAdder.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include <iostream>

int main() {
    logic::Bus<4> bus_a, bus_b, bus_sum;
    logic::Wire carry_in(logic::LogicState::LOW);
    logic::Wire carry_out;

    logic::RippleCarryAdder<4> adder(bus_a, bus_b, carry_in, bus_sum, carry_out);

    bus_a.write_value(7); // 0111
    bus_b.write_value(5); // 0101

    adder.evaluate();

    std::cout << "Sum: " << bus_sum.read_value() << "\n"; // Output: 12 (1100)
    std::cout << "Carry Out: " << static_cast<int>(carry_out.read()) << "\n"; // 0

    return 0;
}
```
