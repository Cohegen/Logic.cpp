#include <cassert>
#include <iostream>

#include "sequential/memory/RAM.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include "signals/wire.hpp"
#include "signals/logicState.hpp"

using namespace logic;

void write_bus_val(Bus<4>& bus, uint8_t val) {
    for (std::size_t i = 0; i < 4; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

void write_addr_val(Bus<2>& bus, uint8_t val) {
    for (std::size_t i = 0; i < 2; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

uint8_t read_bus_val(const Bus<4>& bus) {
    uint8_t val = 0;
    for (std::size_t i = 0; i < 4; ++i) {
        if (bus[i].read() == LogicState::HIGH) {
            val |= (1 << i);
        }
    }
    return val;
}

int main() {
    Clock clock;
    Wire reset{LogicState::LOW};
    Wire read_enable{LogicState::LOW};
    Wire write_enable{LogicState::LOW};

    Bus<2> address;
    Bus<4> write_data;
    Bus<4> read_data;

    RAM<2, 4> ram(clock, reset, read_enable, write_enable, address, write_data, read_data);

    std::cout << "Testing RAM<2, 4> (4 words, 4 bits per word)...\n";

    // 1. Initially read_enable is LOW -> read_data should be 0
    read_enable.write(LogicState::LOW);
    ram.evaluate();
    assert(read_bus_val(read_data) == 0);
    std::cout << "[PASS] Disable read outputs 0\n";

    // 2. Enable read on address 0
    write_addr_val(address, 0);
    read_enable.write(LogicState::HIGH);
    ram.evaluate();
    assert(read_bus_val(read_data) == 0);
    std::cout << "[PASS] Address 0 initial value is 0\n";

    // 3. Write data 0b1010 (10) to Address 1
    write_addr_val(address, 1);
    write_bus_val(write_data, 10);
    write_enable.write(LogicState::HIGH);

    // Setup setup phase
    ram.evaluate();

    // Pulse clock high (rising edge) then low
    clock.tick(); // HIGH
    ram.evaluate();
    clock.tick(); // LOW
    ram.evaluate();

    // Disable write
    write_enable.write(LogicState::LOW);
    ram.evaluate();

    // Read back Address 1
    write_addr_val(address, 1);
    ram.evaluate();
    assert(read_bus_val(read_data) == 10);
    std::cout << "[PASS] Write 10 to Address 1 and read back 10\n";

    // 4. Write data 0b0101 (5) to Address 3
    write_addr_val(address, 3);
    write_bus_val(write_data, 5);
    write_enable.write(LogicState::HIGH);
    ram.evaluate();

    clock.tick(); // HIGH
    ram.evaluate();
    clock.tick(); // LOW
    ram.evaluate();

    write_enable.write(LogicState::LOW);
    ram.evaluate();

    // Verify Address 3 is 5
    write_addr_val(address, 3);
    ram.evaluate();
    assert(read_bus_val(read_data) == 5);
    std::cout << "[PASS] Write 5 to Address 3 and read back 5\n";

    // Verify Address 1 remains 10
    write_addr_val(address, 1);
    ram.evaluate();
    assert(read_bus_val(read_data) == 10);
    std::cout << "[PASS] Address 1 value unchanged (10)\n";

    // 5. Test Reset
    reset.write(LogicState::HIGH);
    ram.evaluate();

    clock.tick(); // HIGH
    ram.evaluate();
    clock.tick(); // LOW
    ram.evaluate();

    reset.write(LogicState::LOW);
    ram.evaluate();

    write_addr_val(address, 1);
    ram.evaluate();
    assert(read_bus_val(read_data) == 0);
    std::cout << "[PASS] Reset clears memory to 0\n";

    std::cout << "All RAM tests passed successfully!\n";
    return 0;
}
