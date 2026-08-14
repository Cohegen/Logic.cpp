# Sequential Registers & Shift Registers

This directory contains generic multi-bit storage registers and shift registers implemented using arrays of D flip-flops (`DFlipFlop`) and 2-to-1 multiplexers (`Mux2to1`).

---

## Overview

Registers store multi-bit binary data synchronously across common clock signals. Shift registers allow stored bits to shift position sequentially on each active clock edge.

---

## Components

### 1. `Register<N>` ([`register.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/registers/register.hpp))
A templated $N$-bit Parallel-In / Parallel-Out (PIPO) register.

* **Template Parameter**: `std::size_t N` (bit-width)
* **Inputs**:
  * `Bus<N>& input`: $N$-bit parallel data input
  * `Wire& clock`: Shared clock signal
* **Outputs**:
  * `Bus<N>& output`: $N$-bit parallel data output
* **Description**: Consists of $N$ parallel `DFlipFlop` instances sharing a common clock wire. On each active clock edge, the value present on `input` is stored into `output`.

---

### 2. `ShiftRegisterSISO<N>` ([`ShiftRegisterSISO.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/registers/ShiftRegisterSISO.hpp))
Serial-In / Serial-Out (SISO) Shift Register.

* **Template Parameter**: `std::size_t N` (number of stages)
* **Inputs**:
  * `Wire& serial_in`: Single-bit serial input
  * `Wire& clock`: Shared clock signal
* **Outputs**:
  * `Wire& serial_out`: Single-bit serial output from stage $N-1$
* **Description**: Connects $N$ flip-flops in series ($DFF_0 \rightarrow DFF_1 \rightarrow \dots \rightarrow DFF_{N-1}$). Data enters at `serial_in` and emerges at `serial_out` after $N$ clock cycles.

---

### 3. `ShiftRegisterSIPO<N>` ([`ShiftRegisterSIPO.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/registers/ShiftRegisterSIPO.hpp))
Serial-In / Parallel-Out (SIPO) Shift Register.

* **Template Parameter**: `std::size_t N` (bit-width)
* **Inputs**:
  * `Wire& serial_in`: Single-bit serial input
  * `Wire& clock`: Shared clock signal
* **Outputs**:
  * `Bus<N>& parallel_out`: $N$-bit parallel output exposing all stage contents
* **Description**: Converts serial data into parallel words over $N$ clock cycles.

---

### 4. `ShiftRegisterPISO<N>` ([`ShiftRegisterPISO.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/registers/ShiftRegisterPISO.hpp))
Parallel-In / Serial-Out (PISO) Shift Register.

* **Template Parameter**: `std::size_t N` (bit-width)
* **Inputs**:
  * `Bus<N>& parallel_in`: $N$-bit parallel load data
  * `Wire& serial_in`: Serial shift input when loading is inactive
  * `Wire& load`: Mode control signal (1 = Load Parallel, 0 = Shift Serial)
  * `Wire& clock`: Shared clock signal
* **Outputs**:
  * `Wire& serial_out`: Serial output stream
* **Architecture**: Uses $N$ 2-to-1 multiplexers (`Mux2to1`) at flip-flop inputs to select between parallel load data and shifted previous stage output.

---

## Usage Example

```cpp
#include "sequential/registers/register.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"

using namespace logic;

Clock clock;
Wire clock_wire(clock.state());
Bus<8> in_bus, out_bus;

// Construct 8-bit PIPO register
Register<8> reg(in_bus, clock_wire, out_bus);

// Write value to input bus
for (int i = 0; i < 8; ++i) {
    in_bus[i].write((i % 2 == 0) ? LogicState::HIGH : LogicState::LOW);
}

// Clock tick updates register output
clock_wire.write(clock.state());
reg.evaluate();
clock.tick();
clock_wire.write(clock.state());
reg.evaluate();
```
