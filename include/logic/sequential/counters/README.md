# Sequential Counters

This directory contains modular gate-level implementations of synchronous digital counters constructed with `Register<N>`, combinational adders/subtractors, 2-to-1 multiplexers (`Mux2to1`), and feedback gates.

---

## Overview

Counters are sequential state machines that sequence through pre-determined numeric or bit patterns on consecutive active clock edges. All counters in this module feature synchronous enable and reset control wires.

---

## Components

### 1. `BinaryCounter<N>` ([`BinaryCounter.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/counters/BinaryCounter.hpp))
Synchronous $N$-bit binary up-counter.

* **Datapath**: `Register` $\rightarrow$ `RippleCarryAdder (+1)` $\rightarrow$ Enable Muxes $\rightarrow$ Reset Muxes $\rightarrow$ `Register`
* **Behavior**: Increments by 1 on each active clock edge when `enable` is HIGH and `reset` is LOW. Wraps around from $2^N - 1$ to 0.

---

### 2. `DownCounter<N>` ([`DownCounter.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/counters/DownCounter.hpp))
Synchronous $N$-bit binary down-counter.

* **Datapath**: `Register` $\rightarrow$ `RippleBorrowSubtractor (-1)` $\rightarrow$ Enable Muxes $\rightarrow$ Reset Muxes $\rightarrow$ `Register`
* **Behavior**: Decrements by 1 on each active clock edge when `enable` is HIGH and `reset` is LOW. Wraps around from 0 to $2^N - 1$.

---

### 3. `UpDownCounter<N>` ([`UpDownCounter.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/counters/UpDownCounter.hpp))
Synchronous $N$-bit binary up/down counter.

* **Inputs**: `Wire& up_down` (1 = Up count, 0 = Down count)
* **Datapath**: Uses adder and subtractor with a direction 2-to-1 MUX stage selecting next incremented/decremented value.

---

### 4. `ModuloCounter<N, M>` ([`ModuloCounter.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/counters/ModuloCounter.hpp))
Synchronous Modulo-$M$ counter with terminal count indicator output.

* **Template Parameters**: `std::size_t N` (bit width), `std::size_t M` (modulus limit, $1 \le M \le 2^N$)
* **Outputs**: `Bus<N>& count`, `Wire& terminal_count`
* **Behavior**: Counts up from $0$ to $M - 1$. When count reaches $M - 1$, `terminal_count` becomes HIGH and the counter resets back to 0 on the next clock cycle.

---

### 5. `JohnsonCounter<N>` ([`JohnsonCounter.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/counters/JohnsonCounter.hpp))
Synchronous $N$-bit Johnson (twisted ring) counter.

* **States**: $2N$ distinct states
* **Datapath**: Connects inverted MSB feedback ($\overline{count[N-1]}$) to LSB input ($shifted\_count[0]$) and shifts right.
* **Sequence (4-bit)**: `0000` $\rightarrow$ `0001` $\rightarrow$ `0011` $\rightarrow$ `0111` $\rightarrow$ `1111` $\rightarrow$ `1110` $\rightarrow$ `1100` $\rightarrow$ `1000` $\rightarrow$ `0000`.

---

### 6. `RingCounter<N>` ([`RingCounter.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/counters/RingCounter.hpp))
Synchronous $N$-bit Ring counter (circular shift counter).

* **States**: $N$ one-hot states
* **Reset Value**: `0b000...0001` (one-hot bit 0 HIGH)
* **Sequence (4-bit)**: `0001` $\rightarrow$ `0010` $\rightarrow$ `0100` $\rightarrow$ `1000` $\rightarrow$ `0001`.

---

## Control Signals Summary

| Signal | Active Level | Description |
|:---:|:---:|---|
| `clock` | Rising Edge ($\uparrow$) | Triggers state storage update |
| `reset` | HIGH | Synchronous reset (clears count to 0, or initial one-hot value for Ring Counter) |
| `enable` | HIGH | Enables counting/shifting; when LOW, counter holds current value |

---

## Usage Example

```cpp
#include "sequential/counters/BinaryCounter.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"

using namespace logic;

Clock clock;
Wire enable{LogicState::HIGH};
Wire reset{LogicState::LOW};
Bus<4> count;

BinaryCounter<4> counter(clock, enable, reset, count);

// Step clock cycles
for (int cycle = 0; cycle < 5; ++cycle) {
    counter.evaluate();
    clock.tick();
    counter.evaluate();
    clock.tick();
    counter.evaluate();
}
```
