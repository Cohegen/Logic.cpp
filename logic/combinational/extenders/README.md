# Zero & Sign Extenders

The `logic::combinational::extenders` module provides bit-width extension components commonly used in CPU instruction decoders, ALU immediate generation, and bus interfacing.

---

## Included Components

### 1. `ZeroExtender<InWidth, OutWidth>`
Pads high-order bits from `InWidth` up to `OutWidth` with `LOW (0)`. Useful for extending unsigned integers and immediate constants.

### 2. `SignExtender<InWidth, OutWidth>`
Replicates the Most Significant Bit (MSB sign bit `in[InWidth - 1]`) across all upper bits from `InWidth` up to `OutWidth`. Essential for signed 2's complement extensions (e.g. branch offsets, signed immediates).

---

## Functional Example

```
Input (4-bit): 0b1011 (-5 in 4-bit 2's complement)

ZeroExtender<4, 8> -> Output: 0b00001011 (Unsigned Value: 11)
SignExtender<4, 8> -> Output: 0b11111011 (Signed Value: -5)
```

---

## Usage Example

```cpp
#include "combinational/extenders/SignExtender.hpp"
#include "signals/bus.hpp"

logic::Bus<16> imm16;
logic::Bus<32> imm32;

logic::SignExtender<16, 32> sext(imm16, imm32);

imm16.write_value(0xFFFC); // -4 in 16-bit
sext.evaluate();

// imm32 reads 0xFFFFFFFC (-4 in 32-bit)
```
