#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include <logic/sequential/memory/Memory.hpp>
#include <logic/signals/bus.hpp>
#include <logic/signals/clock.hpp>
#include <logic/signals/wire.hpp>
#include <logic/signals/logicState.hpp>

using namespace logic;

void write_bus8(Bus<8>& bus, uint8_t val) {
    for (std::size_t i = 0; i < 8; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

void write_addr4(Bus<4>& bus, uint8_t val) {
    for (std::size_t i = 0; i < 4; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

uint8_t read_bus8(const Bus<8>& bus) {
    uint8_t val = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        if (bus[i].read() == LogicState::HIGH) {
            val |= (1 << i);
        }
    }
    return val;
}

int main() {
    Clock clock;
    Wire reset{LogicState::LOW};
    Wire request{LogicState::LOW};
    Wire command{LogicState::LOW};
    Wire ready{LogicState::LOW};

    Bus<4> address;
    Bus<8> write_data;
    Bus<8> read_data;

    // ROM region initialized with firmware data (Addresses 0..7)
    std::vector<std::size_t> rom_image = {
        0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80
    };

    Memory<4, 8> mem_sys(
        clock, reset, request, command, ready,
        address, write_data, read_data, rom_image
    );

    std::cout << "Testing Integrated Memory<4, 8> System...\n";

    // 1. Read from ROM Region (Address 3)
    request.write(LogicState::HIGH);
    command.write(LogicState::LOW); // Read
    write_addr4(address, 3);
    mem_sys.evaluate();

    assert(ready.read() == LogicState::HIGH);
    assert(read_bus8(read_data) == 0x40);
    std::cout << "[PASS] ROM region read verified (Address 3 -> 0x40)\n";

    // 2. Write to RAM Region (Address 10 -> RAM Sub-address 2)
    request.write(LogicState::HIGH);
    command.write(LogicState::HIGH); // Write
    write_addr4(address, 10);
    write_bus8(write_data, 0xBE);
    mem_sys.evaluate();

    // Clock pulse to store data into RAM
    clock.tick(); // HIGH
    mem_sys.evaluate();
    clock.tick(); // LOW
    mem_sys.evaluate();

    // 3. Read back from RAM Region (Address 10)
    command.write(LogicState::LOW); // Read
    mem_sys.evaluate();

    assert(ready.read() == LogicState::HIGH);
    assert(read_bus8(read_data) == 0xBE);
    std::cout << "[PASS] RAM region write & readback verified (Address 10 -> 0xBE)\n";

    // 4. Verify ROM region still intact (Address 3 -> 0x40)
    write_addr4(address, 3);
    mem_sys.evaluate();
    assert(read_bus8(read_data) == 0x40);
    std::cout << "[PASS] ROM region unchanged (Address 3 -> 0x40)\n";

    // 5. Test Reset
    reset.write(LogicState::HIGH);
    mem_sys.evaluate();

    clock.tick(); // HIGH
    mem_sys.evaluate();
    clock.tick(); // LOW
    mem_sys.evaluate();

    reset.write(LogicState::LOW);
    mem_sys.evaluate();

    write_addr4(address, 10);
    mem_sys.evaluate();
    assert(read_bus8(read_data) == 0);
    std::cout << "[PASS] Memory system reset verified (RAM cleared to 0)\n";

    std::cout << "All Memory system integration tests passed successfully!\n";
    return 0;
}
