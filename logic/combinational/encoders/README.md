# Binary Encoders & Priority Encoders

The `logic::combinational::encoders` module contains components that encode active multi-line input signals into compact binary code representations.

---

## Included Components

### 1. `Encoder` (4-to-2 Binary Line Encoder)
Encodes 4 one-hot input lines into a 2-bit binary output code along with an active-high `valid` signal.

### 2. `PriorityEncoder`
Encodes $N$ input lines into $\lceil\log_2 N\rceil$ output bits, returning the binary code of the **highest active input index**.

---

## Truth Tables

### 4-to-2 Binary Line Encoder
| $I_3$ | $I_2$ | $I_1$ | $I_0$ | $Y_1$ | $Y_0$ | Valid ($V$) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 0 | 1 | 0 | 0 | 1 |
| 0 | 0 | 1 | 0 | 0 | 1 | 1 |
| 0 | 1 | 0 | 0 | 1 | 0 | 1 |
| 1 | 0 | 0 | 0 | 1 | 1 | 1 |

---

## Usage Example

```cpp
#include "combinational/encoders/PriorityEncoder.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"

logic::Bus<8> inputs;
logic::Bus<3> encoded_out;
logic::Wire valid;

logic::PriorityEncoder<8, 3> penc(inputs, encoded_out, valid);

inputs.write_value(0b00101100); // Highest active bit is index 5
penc.evaluate();

// valid reads HIGH
// encoded_out reads 5 (0b101)
```
