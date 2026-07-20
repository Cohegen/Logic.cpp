# Gate
- A gate is not just a function, it's an object that is connect to wires.
- For example like the AND gate below:
```
        +--------+
A ------|        |
        |  AND   |------ OUT
B ------|        |
        +--------+
```

- The gate does not own the wires.
- It simply reads them and writes to another wire.

- No matter which gate we use be it AND,OR,XOR,NOT and NAND they all do exactly one thing:
```
Read inputs -> Compute -> Write Output
```

## 1.NOT gate
- A NOT gate has:
       1. one input wire 
       2. one output wire
```
        +------+
Input --| NOT  |---- Output
        +------+
```
- The NOT gate is also known as an **inverter** i.e it inverts its input.

