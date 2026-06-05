# Gates

Primitive logic gates for the rest of the project. Everything in `combinational/` builds circuits from this module.

## What lives here

| File | Role |
|------|------|
| `Gates.h` | Declares the `Gates` class |
| `Gates.cpp` | Implements all gate operations |
| `AND.cpp`, `OR.cpp`, `NOT.cpp`, … | Older standalone programs (not used by the build) |

The CMake target `logic_gates` only compiles `Gates.cpp`. The other `.cpp` files are early experiments that read input from the console; they are kept for reference but are not part of the library.

## The `Gates` class

`Gates` holds one or more boolean inputs and evaluates a gate operation on them. Circuits do not inherit from `Gates` — they **compose** it.

### Constructors

```cpp
Gates a{b};              // one input
Gates ab{a, b};          // two inputs
Gates many{true, false, true};  // initializer_list
Gates vec{someVector};   // vector<bool>
```

### Operations

| Method | Inputs | Behavior |
|--------|--------|----------|
| `AND()` | any | `true` only if every input is `true` |
| `OR()` | any | `true` if any input is `true` |
| `NOT()` | exactly 1 | inverts the input |
| `BUFFER()` | exactly 1 | passes the input through |
| `XOR()` | any | odd number of `true` inputs → `true` |
| `NAND()` | any | `NOT(AND())` |
| `NOR()` | any | `NOT(OR())` |
| `XNOR()` | any | `NOT(XOR())` |

`NOT` and `BUFFER` print an error and return `false` if they receive more than one input.

### Example

```cpp
#include "Gates.h"

Gates gate{true, false};
bool result = gate.AND();  // false
bool inverted = Gates{true}.NOT();  // false
```

## Design

- **Single entry point** — one class instead of separate types per gate.
- **Multi-input AND/OR/XOR** — pass as many bits as you need (useful for wider circuits).
- **Foundation layer** — adders, muxes, and decoders in `combinational/` call `Gates` methods to express their logic.
