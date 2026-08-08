# Sequential Flip-Flops

This directory contains gate-level, edge-triggered flip-flop implementations built hierarchically from basic latches.

---

## Overview

Flip-flops are edge-triggered storage elements that sample input values on a clock signal transition (e.g., positive rising edge) and maintain the sampled value until the next active clock edge.

---

## Components

### 1. `DFlipFlop` ([`D_flip_flop.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/flipflops/D_flip_flop.hpp))
A positive-edge-triggered Master-Slave D Flip-Flop constructed hierarchically from:
* 1 $\times$ `NotGate` (inverts the clock input for master stage)
* 2 $\times$ `DLatch` (Master latch and Slave latch)

#### Architecture & Timing Behavior
```
               +-------------------+        +-------------------+
        D ---->|   Master DLatch   |------->|    Slave DLatch   |----> Q
               |  (Enable = ~CLK)  | Master |   (Enable = CLK)  |
               +-------------------+   Q    +-------------------+----> Q_bar
                         ^                            ^
                         |                            |
                     ~Clock                        Clock
```

1. **Clock LOW ($CLK = 0$)**:
   * Master latch enable ($\bar{CLK}$) is HIGH $\rightarrow$ Master latch is transparent and samples input $D$.
   * Slave latch enable ($CLK$) is LOW $\rightarrow$ Slave latch is opaque and holds previous output state $Q$.
2. **Clock Transition (LOW $\rightarrow$ HIGH)**:
   * Master latch becomes opaque, freezing the sampled $D$ state.
   * Slave latch becomes transparent, transferring the master state to output $Q$.
3. **Clock HIGH ($CLK = 1$)**:
   * Master latch remains opaque, isolating changes on input $D$.
   * Output $Q$ remains stable.

#### Truth Table (Rising Edge)
| $CLK$ | $D$ | $Q_{next}$ | $\bar{Q}_{next}$ | Action |
|:---:|:---:|:---:|:---:|:---:|
| $\uparrow$ | 0 | 0 | 1 | Store 0 |
| $\uparrow$ | 1 | 1 | 0 | Store 1 |
| 0 or 1 | X | $Q$ | $\bar{Q}$ | Hold state |

---

## Usage Example

```cpp
#include "sequential/flipflops/D_flip_flop.hpp"
#include "signals/wire.hpp"
#include "signals/clock.hpp"

using namespace logic;

Clock clock;
Wire d{LogicState::HIGH};
Wire q, q_bar;
Wire clock_wire(clock.state());

DFlipFlop dff(d, clock_wire, q, q_bar);

// Evaluate before tick (Clock LOW)
clock_wire.write(clock.state());
dff.evaluate();

// Trigger rising clock edge (LOW -> HIGH)
clock.tick();
clock_wire.write(clock.state());
dff.evaluate();

// q.read() == LogicState::HIGH
```
