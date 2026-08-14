#include <cassert>
#include <cstdint>
#include <iostream>

#include "sequential/memory/RegisterFile.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include "signals/wire.hpp"
#include "signals/logicState.hpp"

using namespace logic;

void write_bus_val(Bus<8>& bus, uint8_t val) {
    for (std::size_t i = 0; i < 8; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

void write_addr_val(Bus<3>& bus, uint8_t val) {
    for (std::size_t i = 0; i < 3; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

uint8_t read_bus_val(const Bus<8>& bus) {
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
    Wire write_enable{LogicState::LOW};

    Bus<3> read_addr1;
    Bus<3> read_addr2;
    Bus<3> write_addr;
    Bus<8> write_data;
    Bus<8> read_data1;
    Bus<8> read_data2;

    RegisterFile<3, 8> rf(
        clock, reset, write_enable,
        read_addr1, read_addr2, write_addr,
        write_data, read_data1, read_data2
    );

    std::cout << "Testing RegisterFile<3, 8> (8 registers, 8 bits per register)...\n";

    // 1. Initial values are 0
    write_addr_val(read_addr1, 0);
    write_addr_val(read_addr2, 1);
    rf.evaluate();
    assert(read_bus_val(read_data1) == 0);
    assert(read_bus_val(read_data2) == 0);
    std::cout << "[PASS] Initial register values are 0\n";

    // 2. Write 0x55 to R2
    write_addr_val(write_addr, 2);
    write_bus_val(write_data, 0x55);
    write_enable.write(LogicState::HIGH);
    rf.evaluate();

    clock.tick(); // HIGH
    rf.evaluate();
    clock.tick(); // LOW
    rf.evaluate();
    write_enable.write(LogicState::LOW);

    // 3. Write 0xAA to R5
    write_addr_val(write_addr, 5);
    write_bus_val(write_data, 0xAA);
    write_enable.write(LogicState::HIGH);
    rf.evaluate();

    clock.tick(); // HIGH
    rf.evaluate();
    clock.tick(); // LOW
    rf.evaluate();
    write_enable.write(LogicState::LOW);

    // 4. Simultaneous dual read: Port 1 reads R2 (0x55), Port 2 reads R5 (0xAA)
    write_addr_val(read_addr1, 2);
    write_addr_val(read_addr2, 5);
    rf.evaluate();

    assert(read_bus_val(read_data1) == 0x55);
    assert(read_bus_val(read_data2) == 0xAA);
    std::cout << "[PASS] Dual simultaneous read verified (R2=0x55, R5=0xAA)\n";

    // 5. Simultaneous read of same register (R2 on both ports)
    write_addr_val(read_addr1, 2);
    write_addr_val(read_addr2, 2);
    rf.evaluate();

    assert(read_bus_val(read_data1) == 0x55);
    assert(read_bus_val(read_data2) == 0x55);
    std::cout << "[PASS] Simultaneous read of same register (R2 on Port 1 & 2) verified\n";

    // 6. Test Reset
    reset.write(LogicState::HIGH);
    rf.evaluate();

    clock.tick(); // HIGH
    rf.evaluate();
    clock.tick(); // LOW
    rf.evaluate();

    reset.write(LogicState::LOW);
    rf.evaluate();

    write_addr_val(read_addr1, 2);
    write_addr_val(read_addr2, 5);
    rf.evaluate();

    assert(read_bus_val(read_data1) == 0);
    assert(read_bus_val(read_data2) == 0);
    std::cout << "[PASS] Reset clears all registers to 0\n";

    std::cout << "All RegisterFile tests passed successfully!\n";
    return 0;
}
