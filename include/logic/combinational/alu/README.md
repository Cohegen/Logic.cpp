# Arithmetic Logic Unit (ALU)

The `logic::combinational::alu` module provides a multi-functional parameterized Arithmetic Logic Unit (`ALU<Width>`) capable of performing core arithmetic, logical, and bit-shift operations on N-bit operands.

---

## Component Architecture

```
                    +------------------------------------+
       Operand A -->|                                    |
       Operand B -->|                                    |--> Result Bus
  Opcode Control -->|             ALU<Bits>              |
                    |                                    |--> Zero Flag (Z)
                    |                                    |--> Carry Flag (C)
                    |                                    |--> Overflow Flag (V)
                    |                                    |--> Negative Flag (N)
                    +------------------------------------+
```

---

## Supported Operations & Control Codes

The ALU selects its active function based on opcode control wire inputs:

| Opcode / Selection | Operation | Description |
| :---: | :--- | :--- |
| `000` | `ADD` | Binary Addition ($A + B$) |
| `001` | `SUB` | Binary Subtraction ($A - B$) |
| `010` | `AND` | Bitwise AND ($A \land B$) |
| `011` | `OR` | Bitwise OR ($A \lor B$) |
| `100` | `XOR` | Bitwise XOR ($A \oplus B$) |
| `101` | `NOT` | Bitwise NOT ($\bar{A}$) |
| `110` | `SHL` | Logical Shift Left ($A \ll 1$) |
| `111` | `SHR` | Logical Shift Right ($A \gg 1$) |

---

## Status Flags Output Interface

| Flag Wire | Symbol | Condition |
| :--- | :---: | :--- |
| Zero Flag | **Z** | Set `HIGH` if all output result bits are `0`. |
| Carry Flag | **C** | Set `HIGH` if arithmetic addition/subtraction produces an unsigned carry/borrow out. |
| Overflow Flag | **V** | Set `HIGH` if 2's complement signed arithmetic overflow occurs. |
| Negative Flag | **N** | Set `HIGH` if the most significant bit (MSB) of the result is `1`. |

---

## Usage Example

```cpp
#include "combinational/alu/ALU.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include <iostream>

int main() {
    logic::Bus<8> operand_a, operand_b, result;
    logic::Bus<3> control_op;
    logic::Wire flag_z, flag_c, flag_v, flag_n;

    logic::ALU<8> alu(operand_a, operand_b, control_op, result, flag_z, flag_c, flag_v, flag_n);

    // Compute 15 + 1
    operand_a.write_value(15);
    operand_b.write_value(1);
    control_op.write_value(0); // 000 = ADD

    alu.evaluate();

    std::cout << "ALU Result: " << result.read_value() << "\n"; // 16
    std::cout << "Zero Flag: "  << static_cast<int>(flag_z.read()) << "\n"; // 0

    return 0;
}
```
