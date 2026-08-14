# Sequential Memory Modules

This directory contains higher-level storage structures and memory array components.

---

## Components

### `RAM<AddressWidth, DataWidth>`

`RAM<AddressWidth, DataWidth>` is a templated Static Random Access Memory module inheriting from `logic::Component`. It provides $2^{\text{AddressWidth}}$ memory words, each storing `DataWidth` bits of information.

```cpp
template <std::size_t AddressWidth, std::size_t DataWidth>
class RAM : public Component;
```

---

### `ROM<AddressWidth, DataWidth>`

`ROM<AddressWidth, DataWidth>` is a Read-Only Memory component storing pre-loaded binary firmware or data arrays of size $2^{\text{AddressWidth}}$ words, each of width `DataWidth`.

```cpp
template <std::size_t AddressWidth, std::size_t DataWidth>
class ROM : public Component;
```

#### Signal Interface
* `enable` (`Wire&`): Controls output data line tri-state / zero-gating.
* `address` (`Bus<AddressWidth>&`): Address input selecting 1 of $2^{\text{AddressWidth}}$ words.
* `read_data` (`Bus<DataWidth>&`): Output data bus propagating the stored word.

#### Methods & Features
* **Constructor**: `ROM(enable, address, read_data, initial_contents)` accepts an optional `std::vector<std::size_t>` to initialize memory contents.
* **`load_contents(vector)`**: Replaces the ROM image contents.
* **`set_word(index, val)`**: Modifies a word value at a specified address.
* **`get_word(index)`**: Queries the word value stored at an address.

### `RegisterFile<AddressWidth, DataWidth>`

`RegisterFile<AddressWidth, DataWidth>` is a multi-port register array component featuring **dual independent read ports** and a **single synchronous write port**, commonly used in RISC processor pipeline architectures.

```cpp
template <std::size_t AddressWidth, std::size_t DataWidth>
class RegisterFile : public Component;
```

#### Signal Interface
* `clock` (`Clock&`): System clock.
* `reset` (`Wire&`): Reset forcing all registers to `0`.
* `write_enable` (`Wire&`): Global write control wire.
* `read_addr1` (`Bus<AddressWidth>&`): Read Port 1 address selector.
* `read_addr2` (`Bus<AddressWidth>&`): Read Port 2 address selector.
* `write_addr` (`Bus<AddressWidth>&`): Synchronous write address selector.
* `write_data` (`Bus<DataWidth>&`): Data bus to write into target register.
* `read_data1` (`Bus<DataWidth>&`): Output data bus from Read Port 1.
* `read_data2` (`Bus<DataWidth>&`): Output data bus from Read Port 2.

### `MemoryController<AddressWidth, DataWidth>`

`MemoryController<AddressWidth, DataWidth>` is a memory bus controller component managing master CPU request arbitration, address map decoding (ROM vs RAM region selection), read/write control line generation, and ready handshakes.

```cpp
template <std::size_t AddressWidth, std::size_t DataWidth>
class MemoryController : public Component;
```

#### Memory Map & Decoding Scheme
* **ROM Region**: Address MSB == `LOW` (`address[AddressWidth - 1] == LOW`). Asserts `rom_cs`.
* **RAM Region**: Address MSB == `HIGH` (`address[AddressWidth - 1] == HIGH`). Asserts `ram_cs`.

#### Signal Interface
* **CPU Master Side**: `request` (`Wire&`), `command` (`Wire&`, 0=Read, 1=Write), `address` (`Bus<AddressWidth>&`), `write_data` (`Bus<DataWidth>&`), `read_data` (`Bus<DataWidth>&`), `ready` (`Wire&`).
* **Memory Device Side**: `rom_cs` (`Wire&`), `ram_cs` (`Wire&`), `mem_read_en` (`Wire&`), `mem_write_en` (`Wire&`), `mem_address` (`Bus<AddressWidth>&`), `mem_write_data` (`Bus<DataWidth>&`), `mem_read_data` (`Bus<DataWidth>&`).

### `Memory<AddressWidth, DataWidth>`

`Memory<AddressWidth, DataWidth>` is the top-level integrated memory system component. It combines `MemoryController`, `ROM` (mapping to the lower half address space `MSB == 0`), and `RAM` (mapping to the upper half address space `MSB == 1`) into a unified system hierarchy.

```cpp
template <std::size_t AddressWidth, std::size_t DataWidth>
class Memory : public Component;
```

#### Signal Interface
* `clock` (`Clock&`): System clock.
* `reset` (`Wire&`): Global reset.
* `request` (`Wire&`): Memory access request signal.
* `command` (`Wire&`): `LOW` = Read, `HIGH` = Write.
* `ready` (`Wire&`): Memory ready handshake response.
* `address` (`Bus<AddressWidth>&`): System address bus.
* `write_data` (`Bus<DataWidth>&`): Data write bus.
* `read_data` (`Bus<DataWidth>&`): Data read output bus.

---

## Signal Interface

| Port / Wire | Type | Direction | Description |
| :--- | :--- | :--- | :--- |
| `clock` | `Clock&` | Input | System clock driving synchronous memory register updates. |
| `reset` | `Wire&` | Input | Asynchronous/synchronous reset forcing memory content to `0`. |
| `read_enable` | `Wire&` | Input | Controls output read data line tri-state / zero-gating. |
| `write_enable` | `Wire&` | Input | Enables data writing to the addressed word on clock edge. |
| `address` | `Bus<AddressWidth>&` | Input | Multi-bit binary address selecting 1 of $2^{\text{AddressWidth}}$ words. |
| `write_data` | `Bus<DataWidth>&` | Input | Data word written into the selected memory address. |
| `read_data` | `Bus<DataWidth>&` | Output | Data word read from the selected memory address. |

---

## Internal Architecture & Operations

```
                   +---------------------------------------+
 address --------->|           Address Decoder             |
                   +-------------------+-------------------+
                                       |
                                       | word_select_[i]
                                       v
                   +-------------------+-------------------+
 write_enable ---->|           Write Enable &              |
 clock ----------->|         Clock Gating Logic            |
                   +-------------------+-------------------+
                                       |
                                       | gated_clock_[i]
                                       v
                   +-------------------+-------------------+
 write_data ------>|        Register Array [0..N-1]        |
                   |   (N = 2^AddressWidth, W = DataWidth) |
                   +-------------------+-------------------+
                                       |
                                       | memory_words_[i]
                                       v
                   +-------------------+-------------------+
 read_enable ----->|        Output Read Multiplexer        |----> read_data
                   +---------------------------------------+
```

### 1. Address Decoding Stage
The binary address signal bus `Bus<AddressWidth>` is decoded into an array of $N = 2^{\text{AddressWidth}}$ one-hot word selection wires (`word_select_`).
* Address index conversion calculates $\text{index} = \sum_{k=0}^{A-1} \text{address}[k] \cdot 2^k$.
* `word_select_[i]` evaluates `HIGH` if $i == \text{index}$, otherwise `LOW`.

### 2. Word Storage & Clock-Gated Write Logic
Memory storage consists of an array of $N$ instances of `Register<DataWidth>`:
* **Write Enable Gate**: Word $i$ write enable is activated via $\text{word\_write\_enable}[i] = \text{write\_enable} \land \text{word\_select}[i]$.
* **Gated Clocking**: Clock signals to register $i$ are gated: $\text{gated\_clock}[i] = \text{clock\_wire} \land \text{word\_write\_enable}[i]$.
* **Reset Gating**: When `reset` is active (`HIGH`), word write enables are held active and reset multiplexers supply zero values to all registers.

### 3. Read Path Multiplexing
Reading is combinational:
* The selected memory word index outputs data to an internal bus `selected_data_`.
* If `read_enable` is `HIGH`, `selected_data_` is driven directly to output `read_data_`.
* If `read_enable` is `LOW`, `read_data_` is driven to `0` (all `LOW`).

---

## Usage Example

```cpp
#include "sequential/memory/RAM.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"

// Instantiate 4-word (2-bit address), 8-bit data RAM
logic::Clock clock;
logic::Wire reset;
logic::Wire read_enable;
logic::Wire write_enable;

logic::Bus<2> address;
logic::Bus<8> write_data;
logic::Bus<8> read_data;

logic::RAM<2, 8> ram(clock, reset, read_enable, write_enable, address, write_data, read_data);

// Write value 0xAB into address 1
// 1. Set address bus to 1 (0b01)
address[0].write(logic::LogicState::HIGH);
address[1].write(logic::LogicState::LOW);

// 2. Set write_data to 0xAB and assert write_enable
write_enable.write(logic::LogicState::HIGH);
ram.evaluate();

// 3. Pulse clock
clock.tick(); // Rising edge writes to register
ram.evaluate();
clock.tick(); // Falling edge
ram.evaluate();

// 4. Enable read and view output
write_enable.write(logic::LogicState::LOW);
read_enable.write(logic::LogicState::HIGH);
ram.evaluate();
```

