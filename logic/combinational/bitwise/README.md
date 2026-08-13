# Bitwise Operations Subsystem

The `logic::combinational::bitwise` module provides bus-wide parallel bitwise logic operation units.

---

## Components

| Component | Signature | Operation |
| :--- | :--- | :--- |
| `BitwiseAND<Width>` | `(Bus<Width>& A, Bus<Width>& B, Bus<Width>& Out)` | $Out[i] = A[i] \land B[i]$ |
| `BitwiseOR<Width>` | `(Bus<Width>& A, Bus<Width>& B, Bus<Width>& Out)` | $Out[i] = A[i] \lor B[i]$ |
| `BitwiseXOR<Width>` | `(Bus<Width>& A, Bus<Width>& B, Bus<Width>& Out)` | $Out[i] = A[i] \oplus B[i]$ |
| `BitwiseNOT<Width>` | `(Bus<Width>& In, Bus<Width>& Out)` | $Out[i] = \overline{In[i]}$ |

---

## Usage Example

```cpp
#include "combinational/bitwise/BitwiseAND.hpp"
#include "signals/bus.hpp"
#include <iostream>

int main() {
    logic::Bus<8> bus_a, bus_b, bus_out;

    logic::BitwiseAND<8> bitwise_and(bus_a, bus_b, bus_out);

    bus_a.write_value(0b11001100);
    bus_b.write_value(0b10101010);

    bitwise_and.evaluate();

    std::cout << "Bitwise AND: 0b" << std::hex << bus_out.read_value() << "\n"; // 0b10001000 (0x88)

    return 0;
}
```
