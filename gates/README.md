# Gates Module

The `gates` module implements an object-oriented hardware simulation model for primitive digital logic gates.

## Overview

Unlike pure mathematical functions, logic gates in this library are persistent hardware objects connected to `Wire` references. Each gate object reads state values from its connected input wire(s), computes the resulting logic state using 4-valued logic rules (`logicstate.hpp`), and writes the evaluated output state onto its output wire.

```
       +--------+
A ---->|        |
       |  GATE  |----> OUT
B ---->|        |
       +--------+
```
![output:](/assets/gate.png)

### Execution Flow
Every gate inherits from the abstract base class `logic::Gate` and implements the evaluation interface:

$$\text{Read Inputs from Wires} \longrightarrow \text{Compute 4-Valued Logic} \longrightarrow \text{Write Result to Output Wire}$$

---

## Class Hierarchy

```
                   +------------------+
                   |   logic::Gate    |  (Abstract Base Class)
                   +------------------+
                     /      |       \
                    /       |        \
    +-----------------+  +----------+  +-------------------+
    | logic::NotGate  |  |BufferGate|  | logic::BinaryGate | (Abstract Intermediate Base)
    +-----------------+  +----------+  +-------------------+
                                           /    |    |    \    \
                                          /     |    |     \    \
                                    +-------+ +----+ +-----+ +------+ +-------+
                                    |ANDGate| |OrGate| |NandGate| |NorGate| |XorGate| ...
                                    +-------+ +----+ +-----+ +------+ +-------+
```

---

## Core Components

| Header File | Class | Base Class | Description |
|-------------|-------|------------|-------------|
| `Gates.hpp` | `logic::Gate` | N/A | Abstract base class defining `virtual void evaluate() = 0`. |
| `binary_gate.hpp` | `logic::BinaryGate` | `logic::Gate` | Intermediate base class for two-input logic gates. |
| `buffer.hpp` | `logic::BufferGate` | `logic::Gate` | Single-input buffer that copies input state directly to output wire. |
| `NOT.hpp` | `logic::NotGate` | `logic::Gate` | Single-input inverter gate applying `logic_not`. |
| `AND.hpp` | `logic::ANDGate` | `logic::BinaryGate` | Two-input AND gate applying `logic_and`. |
| `OR.hpp` | `logic::OrGate` | `logic::BinaryGate` | Two-input OR gate applying `logic_or`. |
| `NAND.hpp` | `logic::NandGate` | `logic::BinaryGate` | Two-input NAND gate applying `logic_nand`. |
| `NOR.hpp` | `logic::NorGate` | `logic::BinaryGate` | Two-input NOR gate applying `logic_nor`. |
| `XOR.hpp` | `logic::XorGate` | `logic::BinaryGate` | Two-input XOR gate applying `logic_xor`. |
| `XNOR.hpp` | `logic::XnorGate` | `logic::BinaryGate` | Two-input XNOR gate applying `logic_xnor`. |

---

## Detailed Specifications

### 1. Base Classes (`Gates.hpp` & `binary_gate.hpp`)

- **`logic::Gate`**: Defines the pure virtual method `evaluate()`.
- **`logic::BinaryGate`**: Stores references to two input wires (`m_inputA`, `m_inputB`) and one output wire (`m_output`).

### 2. Single-Input Gates

#### Buffer Gate (`buffer.hpp`)
Passes the state of the input wire directly to the output wire.

```cpp
logic::Wire inputWire(logic::LogicState::HIGH);
logic::Wire outputWire(logic::LogicState::LOW);

logic::BufferGate buf(inputWire, outputWire);
buf.evaluate(); // outputWire now reads HIGH
```

#### NOT Gate (`NOT.hpp`)
Inverts the logic state of the input wire using `logic_not()`.

```cpp
logic::Wire in(logic::LogicState::HIGH);
logic::Wire out;

logic::NotGate inverter(in, out);
inverter.evaluate(); // out now reads LOW
```

---

### 3. Two-Input (Binary) Gates

Binary gates receive references to two input wires (`a`, `b`) and one output wire (`out`). Calling `.evaluate()` evaluates their 4-valued logic truth table.

| Gate | Class Name | Function | Truth Table Highlights |
|------|------------|----------|-------------------------|
| **AND** | `logic::ANDGate` | `logic_and` | `1` only if both inputs are `1`; `0` if any input is `0`. |
| **OR** | `logic::OrGate` | `logic_or` | `1` if any input is `1`; `0` if both inputs are `0`. |
| **NAND** | `logic::NandGate` | `logic_nand` | Inverse of AND; `0` only if both inputs are `1`. |
| **NOR** | `logic::NorGate` | `logic_nor` | Inverse of OR; `1` only if both inputs are `0`. |
| **XOR** | `logic::XorGate` | `logic_xor` | `1` if inputs differ; `0` if inputs are identical. |
| **XNOR** | `logic::XnorGate` | `logic_xnor` | `1` if inputs are identical; `0` if inputs differ. |

---

## Example Usage

Connecting primitive gates into higher-level circuits:

```cpp
#include "Signals/wire.hpp"
#include "gates/AND.hpp"
#include "gates/NOT.hpp"
#include <iostream>

int main()
{
    using namespace logic;

    Wire a(LogicState::HIGH);
    Wire b(LogicState::HIGH);
    Wire andOut;
    Wire notOut;

    ANDGate andGate(a, b, andOut);
    NotGate notGate(andOut, notOut);

    andGate.evaluate();
    notGate.evaluate();

    std::cout << "AND Output: " << andOut.read() << std::endl; // '1'
    std::cout << "NAND Equivalent Output: " << notOut.read() << std::endl; // '0'

    return 0;
}
```
