#include <cassert>
#include <cstdint>
#include <iostream>

#include "sequential/memory/MemoryController.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "signals/logicState.hpp"

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
    Wire request{LogicState::LOW};
    Wire command{LogicState::LOW};
    Wire ready{LogicState::LOW};
    Wire rom_cs{LogicState::LOW};
    Wire ram_cs{LogicState::LOW};
    Wire mem_read_en{LogicState::LOW};
    Wire mem_write_en{LogicState::LOW};

    Bus<4> address;
    Bus<8> write_data;
    Bus<8> read_data;

    Bus<4> mem_address;
    Bus<8> mem_write_data;
    Bus<8> mem_read_data;

    MemoryController<4, 8> ctrl(
        request, command, address, write_data, read_data, ready,
        rom_cs, ram_cs, mem_read_en, mem_write_en,
        mem_address, mem_write_data, mem_read_data
    );

    std::cout << "Testing MemoryController<4, 8>...\n";

    // 1. Idle state (request = LOW)
    request.write(LogicState::LOW);
    ctrl.evaluate();
    assert(ready.read() == LogicState::LOW);
    assert(rom_cs.read() == LogicState::LOW);
    assert(ram_cs.read() == LogicState::LOW);
    std::cout << "[PASS] Idle state verified\n";

    // 2. ROM Read Request (Address = 0b0010 < 8 -> MSB=0)
    request.write(LogicState::HIGH);
    command.write(LogicState::LOW); // Read
    write_addr4(address, 2);
    write_bus8(mem_read_data, 0x42);
    ctrl.evaluate();

    assert(ready.read() == LogicState::HIGH);
    assert(rom_cs.read() == LogicState::HIGH);
    assert(ram_cs.read() == LogicState::LOW);
    assert(mem_read_en.read() == LogicState::HIGH);
    assert(mem_write_en.read() == LogicState::LOW);
    assert(read_bus8(read_data) == 0x42);
    std::cout << "[PASS] ROM Read request & decoding verified (Address 2 -> ROM_CS=1, Data=0x42)\n";

    // 3. RAM Write Request (Address = 0b1010 >= 8 -> MSB=1)
    request.write(LogicState::HIGH);
    command.write(LogicState::HIGH); // Write
    write_addr4(address, 10);
    write_bus8(write_data, 0x99);
    ctrl.evaluate();

    assert(ready.read() == LogicState::HIGH);
    assert(rom_cs.read() == LogicState::LOW);
    assert(ram_cs.read() == LogicState::HIGH);
    assert(mem_read_en.read() == LogicState::LOW);
    assert(mem_write_en.read() == LogicState::HIGH);
    assert(read_bus8(mem_write_data) == 0x99);
    std::cout << "[PASS] RAM Write request & decoding verified (Address 10 -> RAM_CS=1, WriteData=0x99)\n";

    std::cout << "All MemoryController tests passed successfully!\n";
    return 0;
}
