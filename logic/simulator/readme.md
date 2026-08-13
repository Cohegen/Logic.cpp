# Hardware Simulation Engine

The `logic::Simulator` system coordinates the state evaluation, event propagation, and clock cycle management across digital components in a circuit topology.

While individual `logic::Component` instances can be manually evaluated using `.evaluate()`, the `Simulator` automates systemic circuit propagation and multi-component execution loops.

---

## Core Responsibilities

1. **Component Registration**: Maintain an ordered registry of active hardware components (`logic::Component*`).
2. **Evaluation Loops**: Cascade evaluation calls across registered components to reach steady-state logic propagation.
3. **Clock Cycle Orchestration**: Drive `logic::Clock` ticks and trigger synchronized sampling for sequential components.
4. **Time & Delta-Cycle Tracking**: Track total simulated elapsed time in ticks/cycles.

---

## Signal Propagation Model

```
       +----------------------------------------------------+
       |                   Simulator                        |
       +-------------------------+--------------------------+
                                 |
           +---------------------+---------------------+
           |                                           |
           v                                           v
+---------------------+                     +---------------------+
| 1. Step Clock Edge  |                     | 2. Evaluate Logic   |
| (Sample Sequential) |                     |  (Combinational)    |
+----------+----------+                     +----------+----------+
           |                                           |
           +---------------------+---------------------+
                                 |
                                 v
                    +--------------------------+
                    | 3. Check Convergence /   |
                    |    Advance Time Step     |
                    +--------------------------+
```

### Evaluation Strategy
* **Combinational Circuits**: Evaluated repeatedly until signals settle (zero delta changes detected across connected wires).
* **Sequential Circuits**: Sample inputs on active clock transitions (`HIGH` edge), updating storage elements deterministically before propagating state changes to downstream combinational logic.

---

## API Reference

```cpp
namespace logic {

class Simulator {
public:
    Simulator();

    // Register hardware components into simulation workspace
    void add_component(Component* component);
    void remove_component(Component* component);

    // Step clock cycle and propagate signals
    void step(std::size_t steps = 1);
    void run_cycles(std::size_t cycles);

    // Reset simulation environment and state
    void reset();

    // Query simulation state
    [[nodiscard]] std::size_t current_cycle() const noexcept;
};

} // namespace logic
```

---

## Usage Example

```cpp
#include "simulator/simulator.hpp"
#include "sequential/registers/Register.hpp"
#include "combinational/adders/FullAdder.hpp"

logic::Simulator sim;

// Setup hardware
logic::Clock clock;
logic::Wire reset, enable;
logic::Bus<8> d_in, q_out;

logic::Register<8> reg(clock, reset, enable, d_in, q_out);

// Register component with simulator engine
sim.add_component(&reg);

// Run 10 clock cycles
sim.run_cycles(10);
```
