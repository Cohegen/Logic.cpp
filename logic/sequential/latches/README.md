# Sequential Latches

This directory contains gate-level implementations of fundamental level-sensitive storage elements (latches) used as building blocks for higher-level sequential components.

---

## Overview

Latches are level-sensitive memory elements whose output updates based on the state of input control signals while enabled. When disabled, latches retain their previously stored bit state ($Q$ and $\bar{Q}$).

---

## Components

### 1. `SRLatch` ([`SRLatch.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/latches/SRLatch.hpp))
An un-gated Set-Reset latch constructed using two cross-coupled NOR gates.

* **Inputs**: $S$ (Set), $R$ (Reset)
* **Outputs**: $Q$, $\bar{Q}$
* **Truth Table**:
  | $S$ | $R$ | $Q_{next}$ | $\bar{Q}_{next}$ | State / Action |
  |:---:|:---:|:---:|:---:|:---:|
  | 0 | 0 | $Q$ | $\bar{Q}$ | Hold (No Change) |
  | 0 | 1 | 0 | 1 | Reset |
  | 1 | 0 | 1 | 0 | Set |
  | 1 | 1 | 0 | 0 | Invalid (Forbidden) |

---

### 2. `GatedSRLatch` ([`GatedSRLatch.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/latches/GatedSRLatch.hpp))
An SR latch enabled by a control signal. It uses two AND gates to gate the input $S$ and $R$ signals before feeding them to an underlying `SRLatch`.

* **Inputs**: $S$ (Set), $R$ (Reset), $E$ (Enable)
* **Outputs**: $Q$, $\bar{Q}$
* **Behavior**:
  * $E = 0$: Outputs hold state ($Q_{next} = Q$) regardless of $S$ and $R$.
  * $E = 1$: Operates as a standard `SRLatch`.

---

### 3. `DLatch` ([`DLatch.hpp`](file:///c:/Users/HP%20840%20G3/OneDrive/Desktop/Logic.cpp/logic/sequential/latches/DLatch.hpp))
A Data (D) latch constructed using a `NotGate` and a `GatedSRLatch`. It eliminates the invalid $S=1, R=1$ state of SR latches.

* **Inputs**: $D$ (Data), $E$ (Enable)
* **Outputs**: $Q$, $\bar{Q}$
* **Truth Table**:
  | $E$ | $D$ | $Q_{next}$ | State / Action |
  |:---:|:---:|:---:|:---:|
  | 0 | X | $Q$ | Hold (Latch state) |
  | 1 | 0 | 0 | Reset |
  | 1 | 1 | 1 | Set (Transparent) |

---

## Usage Example

```cpp
#include "sequential/latches/DLatch.hpp"
#include "signals/wire.hpp"

using namespace logic;

Wire d{LogicState::HIGH};
Wire enable{LogicState::HIGH};
Wire q, q_bar;

DLatch d_latch(d, enable, q, q_bar);
d_latch.evaluate();

// q.read() == LogicState::HIGH
```
