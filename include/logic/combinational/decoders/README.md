# Decoders & Encoders

The `logic::combinational::decoders` module contains binary code conversion components: Decoders, Encoders, and Priority Encoders.

---

## Components

### 1. Binary Line Decoders
Decodes an $N$-bit binary address into a 1-hot activated output array of $2^N$ lines.

| Component | Input Address | Output Lines | Enable Wire |
| :--- | :---: | :---: | :---: |
| `Decoder2to4` | 2 bits | 4 lines | Optional |
| `Decoder3to8` | 3 bits | 8 lines | Optional |

---

### 2. Encoders & Priority Encoders
Encodes $2^N$ input lines into an $N$-bit binary code. Priority Encoders output the binary code corresponding to the highest-order active input line.

| Component | Input Lines | Output Code | Valid Wire |
| :--- | :---: | :---: | :---: |
| `Encoder4to2` | 4 lines | 2 bits | Yes |
| `PriorityEncoder` | Parameterized ($M$) | $\lceil\log_2 M\rceil$ bits | Yes (`Valid`) |

---

## Truth Tables

### 2:4 Decoder (Active-High Output)
| Enable ($E$) | $A_1$ | $A_0$ | $Y_3$ | $Y_2$ | $Y_1$ | $Y_0$ |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0 | X | X | 0 | 0 | 0 | 0 |
| 1 | 0 | 0 | 0 | 0 | 0 | 1 |
| 1 | 0 | 1 | 0 | 0 | 1 | 0 |
| 1 | 1 | 0 | 0 | 1 | 0 | 0 |
| 1 | 1 | 1 | 1 | 0 | 0 | 0 |

---

## Usage Example

```cpp
#include "combinational/decoders/Decoder2to4.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include <iostream>

int main() {
    logic::Bus<2> address;
    logic::Wire enable(logic::LogicState::HIGH);
    logic::Bus<4> outputs;

    logic::Decoder2to4 decoder(address, enable, outputs);

    // Select address 2 (binary 10)
    address.write_value(2);
    decoder.evaluate();

    std::cout << "Decoder Outputs: 0b" 
              << static_cast<int>(outputs[3].read())
              << static_cast<int>(outputs[2].read())
              << static_cast<int>(outputs[1].read())
              << static_cast<int>(outputs[0].read()) << "\n"; // 0100

    return 0;
}
```
