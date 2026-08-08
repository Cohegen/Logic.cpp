# Sequential Memory Modules

This directory is reserved for higher-level storage structures and memory array components.

---

## Overview

Memory modules provide scalable multi-word binary storage accessible via address buses, control lines (read/write enable), and data input/output ports.

---

## Planned Architecture & Components

### 1. `RegisterFile<NumRegisters, BitWidth>`
A multi-port register file containing $2^A$ words of width $W$.
* **Features**: Dual-read ports, single synchronous write port, register decoders, and output multiplexers.

### 2. `RAM<AddressWidth, DataWidth>` (Random Access Memory)
Static RAM module supporting synchronous write operations and combinational/synchronous read access.
* **Control Wires**: `chip_select`, `write_enable`, `output_enable`, `clock`.

### 3. `ROM<AddressWidth, DataWidth>` (Read-Only Memory)
Combinational or clocked memory pre-loaded with binary firmware/data arrays.

---

## Component Integration Guidelines

All memory modules implemented in this directory follow the `logic::Component` interface:
* Inherit from `logic::Component`.
* Accept signal buses (`Bus<N>&`) and control wires (`Wire&`).
* Implement `evaluate() noexcept override` to process address decoding, read bus propagation, and write-enable latching on clock edges.
