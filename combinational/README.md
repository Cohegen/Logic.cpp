# Combinational Circuits

Higher-level digital logic blocks built from primitive logic gates in `gates/` and composite subcircuits. Every circuit in this module is header-only — simply include the header and wire the components together using the `logic::Wire` and `logic::Bus` signal primitives.

Combinational logic circuits are memoryless systems whose outputs are determined strictly by the present state of their inputs.

---

## Component Architectural Matrix

| Header | Circuit Class | Composition Level | Component Breakdown | Sub-Components / Gates Used |
|--------|---------------|-------------------|---------------------|-----------------------------|
| [`halfAdder.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/halfAdder.hpp) | `HalfAdder` | **Gate Primitive** | 2 Primitive Gates | 1 `XorGate`, 1 `ANDGate` |
| [`fullAdder.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/fullAdder.hpp) | `FullAdder` | **Hierarchical Composite** | 2 Sub-Components + 1 Gate | 2 `HalfAdder`, 1 `OrGate` |
| [`halfSubtractor.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/halfSubtractor.hpp) | `HalfSubtractor` | **Gate Primitive** | 3 Primitive Gates | 1 `NotGate`, 1 `XorGate`, 1 `AndGate` |
| [`fullSubtractor.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/fullSubtractor.hpp) | `FullSubtractor` | **Hierarchical Composite** | 2 Sub-Components + 1 Gate | 2 `HalfSubtractor`, 1 `OrGate` |
| [`Mux2to1.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/Mux2to1.hpp) | `Mux2to1` | **Gate Primitive** | 4 Primitive Gates | 1 `NotGate`, 2 `ANDGate`, 1 `OrGate` |
| [`Mux4to1.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/Mux4to1.hpp) | `Mux4to1` | **Hierarchical Composite** | 3 Sub-Components | 3 `Mux2to1` |
| [`Dec2to4.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/Dec2to4.hpp) | `Dec2to4` | **Gate Primitive** | 6 Primitive Gates | 2 `NotGate`, 4 `AndGate` |
| [`RippleCarryAdder.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/RippleCarryAdder.hpp) | `RippleCarryAdder<N>` | **Templated Cascaded** | $N$ Sub-Components | $N$ `FullAdder` |
| [`Comparator.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/combinational/Comparator.hpp) | `Comparator<N>` | **Templated Cascaded** | $N$ Sub-Components + Bus | $N$ `Comparator1Bit` (each with 2 `NotGate`, 1 `XnorGate`, 2 `ANDGate`) |

---

## Architectural Principles & Composition Patterns

The combinational circuit library follows strict object-oriented modular design rules:

1. **Gate Primitive Level**: Base components (e.g., `HalfAdder`, `HalfSubtractor`, `Mux2to1`, `Dec2to4`, `Comparator1Bit`) instantiate primitive logic gates (`ANDGate`, `OrGate`, `NotGate`, `XorGate`, `XnorGate`) directly to evaluate single-stage logic equations.
2. **Sub-Component Hierarchy**: Multi-stage and complex arithmetic units compose smaller, fully functional sub-components rather than re-implementing raw logic gates.
   - `FullAdder` is composed of two `HalfAdder` subcircuits and an `OrGate`.
   - `FullSubtractor` is composed of two `HalfSubtractor` subcircuits and an `OrGate`.
   - `Mux4to1` is composed of three `Mux2to1` multiplexers.
   - `RippleCarryAdder<N>` cascades $N$ `FullAdder` components via internal carry wires.
3. **Signal Wire Interconnection**: Communication between sub-components uses internal `logic::Wire` members to pass transient signals between stages without global state or pointers.

---

## Circuit Specifications & Implementation Details

### 1. Half Adder (`halfAdder.hpp`)

Adds two 1-bit binary inputs ($A$, $B$) to produce a 1-bit `Sum` and a `Carry` output.

- **Gate Composition**:
  - `XorGate` $\rightarrow$ computes `Sum`
  - `ANDGate` $\rightarrow$ computes `Carry`
- **Boolean Equations**:
  $$\text{Sum} = A \oplus B$$
  $$\text{Carry} = A \cdot B$$

![Half Adder Diagram](assets/halfadder.jfif)

#### Truth Table
| A | B | Sum | Carry |
|---|---|-----|-------|
| 0 | 0 |  0  |   0   |
| 0 | 1 |  1  |   0   |
| 1 | 0 |  1  |   0   |
| 1 | 1 |  0  |   1   |

```cpp
logic::Wire a(logic::LogicState::HIGH), b(logic::LogicState::HIGH);
logic::Wire sum, carry;

logic::HalfAdder adder(a, b, sum, carry);
adder.evaluate(); // sum -> LOW, carry -> HIGH
```

---

### 2. Full Adder (`fullAdder.hpp`)

Adds three 1-bit inputs ($A$, $B$, $C_{in}$) using two `HalfAdder` sub-components and one primitive `OrGate`.

- **Hierarchical Composition**:
  - `HalfAdder 1` ($m\_halfAdder1$): Adds input $A$ and input $B$ $\rightarrow$ produces intermediate sum $S_1$ and intermediate carry $C_1$.
  - `HalfAdder 2` ($m\_halfAdder2$): Adds intermediate sum $S_1$ and carry-in $C_{in}$ $\rightarrow$ produces final output `Sum` and intermediate carry $C_2$.
  - `OrGate` ($m\_carryGate$): ORs intermediate carries $C_1$ and $C_2$ $\rightarrow$ produces final output `Carry`.
- **Internal Signal Routing**:
  $$\text{Sum} = (A \oplus B) \oplus C_{in}$$
  $$\text{Carry} = C_1 \lor C_2 = (A \cdot B) \lor ((A \oplus B) \cdot C_{in})$$

![Full Adder Diagram](assets/fulladder.jfif)

#### Structural Schema
```
          +-----------------+
A ------->|                 |-- S1 --+
B ------->|   HalfAdder 1   |        |    +-----------------+
          |                 |-- C1 --+--->|                 |-- Sum
          +-----------------+        |    |   HalfAdder 2   |
Cin ---------------------------------+--->|                 |-- C2 --+
                                          +-----------------+        |   +--------+
                                                                     +-->| OrGate |-- Carry
                                                                         +--------+
```

```cpp
logic::Wire a(logic::LogicState::HIGH), b(logic::LogicState::LOW), cin(logic::LogicState::HIGH);
logic::Wire sum, carry;

logic::FullAdder adder(a, b, cin, sum, carry);
adder.evaluate(); // sum -> LOW, carry -> HIGH
```

---

### 3. Half Subtractor (`halfSubtractor.hpp`)

Subtracts bit $B$ from bit $A$, yielding a `Difference` output and a `Borrow` output.

- **Gate Composition**:
  - `NotGate` $\rightarrow$ inverts input $A$ ($\bar{A}$)
  - `XorGate` $\rightarrow$ computes `Difference`
  - `AndGate` $\rightarrow$ computes `Borrow` ($\bar{A} \cdot B$)
- **Boolean Equations**:
  $$\text{Difference} = A \oplus B$$
  $$\text{Borrow} = \bar{A} \cdot B$$

![Half Subtractor Diagram](assets/halfsubtractor.png)

#### Truth Table
| A | B | Difference | Borrow |
|---|---|------------|--------|
| 0 | 0 |     0      |   0    |
| 0 | 1 |     1      |   1    |
| 1 | 0 |     1      |   0    |
| 1 | 1 |     0      |   0    |

---

### 4. Full Subtractor (`fullSubtractor.hpp`)

Computes subtraction over three bits ($A - B - B_{in}$) using two `HalfSubtractor` sub-components and one primitive `OrGate`.

- **Hierarchical Composition**:
  - `HalfSubtractor 1` ($m\_sub1$): Subtracts $B$ from $A$ $\rightarrow$ produces intermediate difference $D_1$ and intermediate borrow $B_1$.
  - `HalfSubtractor 2` ($m\_sub2$): Subtracts $B_{in}$ from intermediate difference $D_1$ $\rightarrow$ produces final output `Difference` and intermediate borrow $B_2$.
  - `OrGate` ($or\_gate$): ORs intermediate borrows $B_1$ and $B_2$ $\rightarrow$ produces final output `Borrow`.
- **Internal Signal Routing**:
  $$\text{Difference} = (A \oplus B) \oplus B_{in}$$
  $$\text{Borrow} = B_1 \lor B_2 = (\bar{A} \cdot B) \lor (\overline{(A \oplus B)} \cdot B_{in})$$

![Full Subtractor Diagram](assets/fullSubtractor.jfif)

#### Structural Schema
```
          +-------------------+
A ------->|                   |-- D1 --+
B ------->|  HalfSubtractor 1 |        |    +-------------------+
          |                   |-- B1 --+--->|                   |-- Difference
          +-------------------+        |    |  HalfSubtractor 2 |
Bin -----------------------------------+--->|                   |-- B2 --+
                                            +-------------------+        |   +--------+
                                                                         +-->| OrGate |-- Borrow
                                                                             +--------+
```

---

### 5. 2-to-1 Multiplexer (`Mux2to1.hpp`)

Routes one of two data inputs ($A$ or $B$) to the output line based on a single select signal ($S$).

- **Gate Composition**:
  - 1 `NotGate` $\rightarrow$ inverts select line ($\bar{S}$)
  - 2 `ANDGate` $\rightarrow$ computes product terms ($A \cdot \bar{S}$ and $B \cdot S$)
  - 1 `OrGate` $\rightarrow$ combines product terms
- **Boolean Equation**:
  $$Y = (A \cdot \bar{S}) \lor (B \cdot S)$$

![2-to-1 Mux Diagram](assets/mux2to1.jfif)

#### Truth Table
| Select ($S$) | Output ($Y$) |
|--------------|--------------|
|      0       |   Input $A$  |
|      1       |   Input $B$  |

---

### 6. 4-to-1 Multiplexer (`Mux4to1.hpp`)

Selects one of four data inputs ($A$, $B$, $C$, $D$) using two select lines ($S_0$, $S_1$).

- **Hierarchical Composition**:
  - `Mux2to1` ($m\_mux0$): Selects between $A$ and $B$ using $S_0$ $\rightarrow$ output $I_0$.
  - `Mux2to1` ($m\_mux1$): Selects between $C$ and $D$ using $S_0$ $\rightarrow$ output $I_1$.
  - `Mux2to1` ($m\_mux2$): Selects between $I_0$ and $I_1$ using $S_1$ $\rightarrow$ final `Output`.

![4-to-1 Mux Diagram](assets/mux4to1.jfif)

#### Structural Schema
```
A --+   +----------+
    |-->| Mux2to1  |-- I0 --+
B --+   | (Sel: S0)|        |   +----------+
S0 ---->+----------+        +-->| Mux2to1  |
C --+   +----------+        |   | (Sel: S1)|----> Output
    |-->| Mux2to1  |-- I1 --+   +----------+
D --+   | (Sel: S0)|                 ^
S0 ---->+----------+                 |
S1 ----------------------------------+
```

---

### 7. 2-to-4 Decoder (`Dec2to4.hpp`)

Decodes a 2-bit address ($A$, $B$) into four active-HIGH one-hot outputs ($Y_0, Y_1, Y_2, Y_3$).

- **Gate Composition**:
  - 2 `NotGate` $\rightarrow$ produces inverted lines $\bar{A}$ and $\bar{B}$.
  - 4 `AndGate` $\rightarrow$ computes minterms for each output state.
- **Boolean Equations**:
  $$Y_0 = \bar{A} \cdot \bar{B}$$
  $$Y_1 = \bar{A} \cdot B$$
  $$Y_2 = A \cdot \bar{B}$$
  $$Y_3 = A \cdot B$$

![2-to-4 Decoder Diagram](assets/dec2to4.png)

---

### 8. $N$-Bit Ripple Carry Adder (`RippleCarryAdder.hpp`)

Cascades $N$ `FullAdder` subcircuits to perform arbitrary-width binary addition.

- **Hierarchical Templated Structure**:
  - Instantiates `std::array<std::unique_ptr<FullAdder>, N>`.
  - Connects the `carryOut` wire of stage $i$ directly to the `carryIn` wire of stage $i+1$.
- **Carry Propagation Chain**:
  $$C_0 = C_{in}$$
  $$\text{FullAdder}_i(A_i, B_i, C_i) \longrightarrow (\text{Sum}_i, C_{i+1})$$
  $$C_{out} = C_N$$

![Ripple Carry Adder Diagram](assets/ripple_carry_adder.jfif)

```cpp
std::array<logic::Wire, 4> a, b, sum;
logic::Wire cin(logic::LogicState::LOW), cout;

logic::RippleCarryAdder<4> adder4bit(a, b, cin, sum, cout);
adder4bit.evaluate();
```

---

### 9. $N$-Bit Magnitude Comparator (`Comparator.hpp`)

Compares two $N$-bit buses ($A$ and $B$) to evaluate status outputs: `Equal` ($A=B$), `Greater` ($A>B$), and `Less` ($A<B$).

- **Hierarchical Composition**:
  - `Comparator1Bit`: Evaluates bitwise equality ($A_i \odot B_i$ via `XnorGate`), bitwise greater ($A_i \cdot \bar{B}_i$ via `ANDGate`), and bitwise less ($\bar{A}_i \cdot B_i$ via `ANDGate`).
  - `Comparator<N>`: Aggregates bit-level comparative signals across $N$-bit `Bus<N>` signals from Most Significant Bit (MSB) to Least Significant Bit (LSB).

![Comparator Diagram](assets/comparator.jfif)

---

## Summary of Include Paths

```cpp
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "Signals/bus.hpp"

// Combinational Components
#include "halfAdder.hpp"
#include "fullAdder.hpp"
#include "halfSubtractor.hpp"
#include "fullSubtractor.hpp"
#include "Mux2to1.hpp"
#include "Mux4to1.hpp"
#include "Dec2to4.hpp"
#include "RippleCarryAdder.hpp"
#include "Comparator.hpp"
```

