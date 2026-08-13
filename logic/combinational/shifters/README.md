# Binary Shifters & Barrel Shifter

The `logic::combinational::shifters` module provides digital bit-shifting components for logical, arithmetic, and rotation operations across multi-bit buses.

---

## Included Components

| Component | Class Signature | Description |
| :--- | :--- | :--- |
| `LogicalLeftShifter` | `LogicalLeftShifter<Width>(in, shift_amount, out)` | Shifts bus left by $N$ bits, zero-filling LSBs ($Y = X \ll N$). |
| `LogicalRightShifter` | `LogicalRightShifter<Width>(in, shift_amount, out)` | Shifts bus right by $N$ bits, zero-filling MSBs ($Y = X \gg N$). |
| `ArithmeticRightShifter` | `ArithmeticRightShifter<Width>(in, shift_amount, out)` | Arithmetic right shift replicating MSB sign bit ($Y = X \gg_{\text{arith}} N$). |
| `BarrelShifter` | `BarrelShifter<Width, ShiftBits>(in, shift_amount, mode, out)` | Multi-function shift/rotate unit supporting LLS, LRS, ARS, and ROR. |

---

## Barrel Shifter Mode Select Table

| `mode[1..0]` Code | Mode Name | Behavior |
| :---: | :--- | :--- |
| `00` | Logical Left Shift (LLS) | Out = `In << shift_amount` (0-fill low bits) |
| `01` | Logical Right Shift (LRS) | Out = `In >> shift_amount` (0-fill high bits) |
| `10` | Arithmetic Right Shift (ARS) | Out = `In >> shift_amount` (Sign-fill high bits) |
| `11` | Rotate Right (ROR) | Out = `RotateRight(In, shift_amount)` |

---

## Signal Interface

```
                    +------------------------------------+
       In Bus ------>|                                    |
 Shift Amount ------>|            BarrelShifter           |-----> Out Bus
   Mode (2-bit) ---->|                                    |
                    +------------------------------------+
```

---

## Usage Example

```cpp
#include "combinational/shifters/BarrelShifter.hpp"
#include "signals/bus.hpp"

logic::Bus<8> in_bus, out_bus;
logic::Bus<3> shift_amount;
logic::Bus<2> mode;

logic::BarrelShifter<8, 3> barrel(in_bus, shift_amount, mode, out_bus);

in_bus.write_value(0b11001001);   // 0xC9
shift_amount.write_value(3);
mode.write_value(3);              // Mode 3: Rotate Right

barrel.evaluate();
// out_bus contains 0b00111001 (0x39)
```
