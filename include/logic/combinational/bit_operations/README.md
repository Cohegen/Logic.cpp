# Bit Extraction & Slicing Operations

The `logic::combinational::bit_operations` module provides bus sub-field indexing and bit slicing hardware units.

---

## Included Components

### 1. `BitSelector<Width>`
Extracts a single wire state from input bus `InBus` at index `bit_index` onto a destination `out_wire`.

### 2. `BitSlice<InWidth, StartBit, SliceWidth>`
Extracts a contiguous range of `SliceWidth` bits starting from `StartBit` on an `InWidth` input bus onto a destination `SliceWidth` bus. Ideal for extracting CPU instruction opcodes, register specifiers, or memory offsets.

---

## Architecture Diagram

```
           Input Bus (32-bit Instruction)
   +-------------------------------------------------+
   | 31..26: Opcode | 25..21: Rs | 20..16: Rt | ...  |
   +-------------------------------------------------+
                           |
            BitSlice<32, 26, 6> (Extract Opcode)
                           |
                           v
              Opcode Bus (6-bit Output)
```

---

## Usage Example

```cpp
#include "combinational/bit_operations/BitSlice.hpp"
#include "signals/bus.hpp"

logic::Bus<32> instruction;
logic::Bus<6> opcode;

// Extract 6-bit opcode from bits 26..31
logic::BitSlice<32, 26, 6> slice_op(instruction, opcode);

instruction.write_value(0xFC000000);
slice_op.evaluate();

// opcode reads 0x3F (0b111111)
```
