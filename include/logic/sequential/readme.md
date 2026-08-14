# Notice: Sequential Timing Model Limitation

## Current Behavior

The current implementation of sequential circuits (e.g., `DFlipFlop`, `Register`, and `ShiftRegister`) evaluates components sequentially within a single call to `evaluate()`.

For example:

```cpp
dff0.evaluate();
dff1.evaluate();
dff2.evaluate();
dff3.evaluate();
```

This means that the output of an earlier flip-flop may be visible to a later flip-flop during the same evaluation pass.

For instance, in a shift register:

```
DFF0 → DFF1 → DFF2 → DFF3
```

`DFF1` may immediately observe the updated output of `DFF0` instead of the value that existed before the clock transition.

---

## Why This Is Incorrect

Real digital hardware is synchronous.

On the active clock edge, **every flip-flop samples its input simultaneously**. Only after all inputs have been sampled do the outputs change.

Conceptually, hardware behaves like this:

```
Clock Edge
    │
    ├── All flip-flops sample D inputs
    │
    └── All Q outputs update simultaneously
```

No flip-flop should observe another flip-flop's newly updated output during the same clock edge.

---

## Current Status

This simplified evaluation model is intentional during the early stages of the project.

The current objective is to:

* Build reusable hardware components.
* Verify logical correctness.
* Establish a clean hierarchical hardware architecture.

Accurate timing behavior is intentionally deferred until a dedicated simulation engine is introduced.

---

## Future Improvement

A future version of Logic.cpp should implement an event-driven simulation engine.

Possible approaches include:

* Two-phase evaluation (sample → commit).
* Double-buffered signal values (current/next state).
* Event scheduling with propagation delays.
* Clock-edge detection.
* Delta-cycle simulation similar to professional HDL simulators.

With such a simulator:

1. Every sequential component samples its inputs.
2. Outputs are committed simultaneously.
3. Combinational logic propagates until the circuit reaches a stable state.
4. The simulator advances to the next clock cycle.

This will accurately model real synchronous digital hardware and eliminate artifacts caused by sequential evaluation.

---

## Affected Components

This limitation currently affects all sequential components, including but not limited to:

* `SRLatch`
* `GatedSRLatch`
* `DLatch`
* `DFlipFlop`
* `Register`
* `ShiftRegister`
* Future counters
* Future register files
* Future memory modules

---

## Design Decision

This behavior is **not considered a bug** at the current stage of development. It is a known architectural limitation accepted in favor of keeping the initial implementation simple and focused on hardware composition.

The simulator will be redesigned in a later milestone without requiring major changes to the hardware component interfaces.
