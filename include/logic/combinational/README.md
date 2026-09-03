# Combinational Circuits Subsystem

The `logic::combinational` module contains pure memoryless digital logic components. In combinational circuits, outputs depend strictly and instantaneously on current input values.

---

## Directory Modules

| Directory | Components | Functional Description |
| :--- | :--- | :--- |
| [`adders/`](adders/) | `HalfAdder`, `FullAdder`, `RippleCarryAdder` | Binary addition circuits with sum and carry propagation. |
| [`subtractors/`](subtractors/) | `HalfSubtractor`, `FullSubtractor` | Binary subtraction with difference and borrow propagation. |
| [`alu/`](alu/) | `ALU<Width>` | Multi-function Arithmetic Logic Unit with status flags. |
| [`multiplexers/`](multiplexers/) | `Mux2to1`, `Mux3to1`, `Mux4to1`, `Mux8to1`, `Mux<N>`, `Mux3<N>`, `Mux4<N>` | Data selection routing and demultiplexing units. |
| [`decoders/`](decoders/) | `Decoder2to4`, `Decoder3to8`, `Encoder4to2`, `PriorityEncoder` | Binary line decoders and priority encoders. |
| [`comparators/`](comparators/) | `BitComparator`, `MagnitudeComparator<Width>` | Greater-than, less-than, and equal-to binary comparators. |
| [`bitwise/`](bitwise/) | `BitwiseAND`, `BitwiseOR`, `BitwiseXOR`, `BitwiseNOT` | Bus-wide bitwise operation units. |

---

## Combinational Circuit Characteristics

1. **No Memory / State**: Combinational units contain no clock inputs, latches, or feedback loops.
2. **Instantaneous Propagation**: Calling `.evaluate()` evaluates inputs through constituent logic gates and updates output wires in a single pass.
3. **Reusability**: Combinational components serve as the building blocks for CPU ALUs, memory address decoders, multiplexer routing, and control logic.

---

## Subsystem Architecture Overview

```
                        +----------------------+
                        |   logic::Component   |
                        +----------+-----------+
                                   |
    +------------------------------+------------------------------+
    |               |              |               |              |
    v               v              v               v              v
+-------+     +-----------+     +-----+     +--------------+  +----------+
|Adders |     |Subtractors|     | ALU |     |Multiplexers/ |  |Decoders/ |
+-------+     +-----------+     +-----+     |Demultiplexers|  |Encoders  |
                                            +--------------+  +----------+
```
