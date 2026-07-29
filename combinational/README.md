# Combinational circuits

Higher-level digital blocks built from the primitive `Gates` class in `gates/`. Each file is a header-only circuit — include it and link against `logic_gates`.

Combinational logic means the output depends only on the current inputs, with no memory of past states.

## What lives here

| File | Circuit | Inputs | Outputs |
|------|---------|--------|---------|
| `halfAdder.hpp` | Half adder | 2 bits (A, B) | sum, carry |
| `fullAdder.hpp` | Full adder | 3 bits (A, B, Cin) | Sum, Carry |
| `halfSubtractor.hpp` | Half subtractor | 2 bits (A, B) | Difference, Borrow |
| `fullSubtractor.hpp` | Full subtractor | 3 bits (A, B, Bin) | Difference, Borrow |
| `Mux2to1.hpp` | 2:1 multiplexer | 2 data lines + 1 select line | 1 selected line |
| `Mux4to1.hpp` | 4:1 multiplexer | 4 data lines + 2 select lines | 1 selected line |
| `Dec2to4.hpp` | 2:4 decoder | 2 address bits | 4 one-hot outputs |
| `RippleCarryAdder.hpp` | N-bit Ripple Carry Adder | N-bit A, N-bit B, Cin | N-bit Sum, Cout |
| `Comparator.hpp` | N-bit magnitude comparator | N-bit A, N-bit B | Equal, Greater, Less |

## Design pattern

Every circuit **uses** `Gates`; none inherit from it.

```
Gates (AND, OR, NOT, XOR, …)
  ↓ composed into
HalfAdder, FullAdder, Multiplexer, Decoder
```

Typical patterns in this folder:

- **Member composition** — `HalfAdder` keeps a `Gates` object for its two inputs.
- **Local `Gates` objects** — `FullAdder` and `Multiplexer` create `Gates` inside methods where needed.
- **Input validation** — constructors and output methods check expected input counts and log errors to `stderr`.

---

## Half adder

Adds two bits without a carry-in.

| A | B | sum | carry |
|---|---|-----|-------|
| 0 | 0 | 0 | 0 |
| 0 | 1 | 1 | 0 |
| 1 | 0 | 1 | 0 |
| 1 | 1 | 0 | 1 |

```cpp
HalfAdder adder{true, true};
adder.sum();    // XOR(A, B)
adder.carry();  // AND(A, B)
```
![output:](https://github.com/Cohegen/Logic.cpp/blob/main/combinational/assets/halfadder.jfif)

---

## Full adder

Adds three bits: two operands plus carry-in.

```cpp
FullAdder adder{true, false, true};  // A, B, Cin
adder.Sum();    // XOR of all three inputs
adder.Carry();  // majority function: AB ∨ BC ∨ AC
```

Expects exactly 3 inputs.

![output:](https://github.com/Cohegen/Logic.cpp/blob/main/combinational/assets/fulladder.jfif)

---

## 4:1 multiplexer

Picks one of four data inputs using two select lines (`s0`, `s1`).

| s1 | s0 | output |
|----|----|--------|
| 0 | 0 | `inputs[0]` |
| 0 | 1 | `inputs[1]` |
| 1 | 0 | `inputs[2]` |
| 1 | 1 | `inputs[3]` |

```cpp
Multiplexer mux{false, true, false, true};
mux.setSelectLines(true, false);  // s0=1, s1=0 → selects inputs[2]
mux.output();
```

Implemented with minterms and `Gates` (AND/OR/NOT), not raw `if/else`.

![output:](https://github.com/Cohegen/Logic.cpp/blob/main/combinational/assets/mux4to1.jfif)

---

## 2:4 decoder

Turns a 2-bit address into four one-hot outputs. Only one output is `true` at a time.

| inputs[0] | inputs[1] | active output |
|-----------|-----------|---------------|
| 0 | 0 | `out[0]` |
| 0 | 1 | `out[1]` |
| 1 | 0 | `out[2]` |
| 1 | 1 | `out[3]` |

```cpp
Decoder decoder{false, true};
auto out = decoder.outputs();  // {false, true, false, false}
```

Each output is a minterm built from `NOT` and `AND`:

- `out[0]` = ¬A0 ∧ ¬A1
- `out[1]` = ¬A0 ∧ A1
- `out[2]` = A0 ∧ ¬A1
- `out[3]` = A0 ∧ A1

---

## Including in your code

Both `gates/` and `combinational/` are on the include path via CMake:

```cpp
#include "Gates.h"
#include "halfAdder.hpp"
```

`src/main.cpp` currently demos `Gates` and `HalfAdder`. The other circuits are ready to include the same way.
