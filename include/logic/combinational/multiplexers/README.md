# Multiplexers & Demultiplexers

The `logic::combinational::multiplexers` module provides digital data routing hardware primitives: Multiplexers (Mux) and Demultiplexers (Demux).

---

## Components

### 1. Multiplexers (Data Selectors)
A Multiplexer routes one of $2^S$ data input lines to a single output line based on $S$ select lines.

| Component | Input Lines | Select Lines | Output Lines |
| :--- | :---: | :---: | :---: |
| `Mux2to1` | 2 | 1 | 1 |
| `Mux3to1` | 3 | 2 | 1 |
| `Mux4to1` | 4 | 2 | 1 |
| `Mux8to1` | 8 | 3 | 1 |
| `Mux<N>` | 2 (buses of size N) | 1 | 1 (bus of size N) |
| `Mux3<N>` | 3 (buses of size N) | 2 | 1 (bus of size N) |
| `Mux4<N>` | 4 (buses of size N) | 2 | 1 (bus of size N) |

#### Mux Boolean Equation (2:1)
$$Y = (\bar{S} \cdot I_0) + (S \cdot I_1)$$

---

### 2. Demultiplexers (Data Distributers)
A Demultiplexer routes a single input data line to one of $2^S$ output lines according to $S$ select control lines.

| Component | Input Lines | Select Lines | Output Lines |
| :--- | :---: | :---: | :---: |
| `Demux1to2` | 1 | 1 | 2 |
| `Demux1to4` | 1 | 2 | 4 |
| `Demux1to8` | 1 | 3 | 8 |

---

## Architecture Diagrams

```
         4:1 Multiplexer                       1:4 Demultiplexer
       +-----------------+                   +-----------------+
 I0 -->|                 |             Out0 <|                 |
 I1 -->|                 |             Out1 <|                 |
 I2 -->|     Mux4to1     |--> Output   Out2 <|    Demux1to4    |<-- Input
 I3 -->|                 |             Out3 <|                 |
       +--------+--------+                   +--------+--------+
                ^                                     ^
                | Select Bus (2-bit)                  | Select Bus (2-bit)
```

---

## Usage Example

```cpp
#include "combinational/multiplexers/Mux4to1.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include <iostream>

int main() {
    logic::Wire i0(logic::LogicState::LOW);
    logic::Wire i1(logic::LogicState::HIGH);
    logic::Wire i2(logic::LogicState::LOW);
    logic::Wire i3(logic::LogicState::LOW);

    logic::Bus<2> sel;
    logic::Wire output;

    logic::Mux4to1 mux(i0, i1, i2, i3, sel, output);

    // Select input line 1 (sel = 01)
    sel.write_value(1);
    mux.evaluate();

    std::cout << "Mux Output: " << static_cast<int>(output.read()) << "\n"; // Output: 1

    return 0;
}
```
