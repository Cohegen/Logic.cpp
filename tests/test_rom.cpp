#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include <logic/sequential/memory/ROM.hpp>
#include <logic/signals/bus.hpp>
#include <logic/signals/wire.hpp>
#include <logic/signals/logicState.hpp>

using namespace logic;

void write_addr(Bus<3>& bus, uint8_t val) {
    for (std::size_t i = 0; i < 3; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

uint8_t read_data_val(const Bus<8>& bus) {
    uint8_t val = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        if (bus[i].read() == LogicState::HIGH) {
            val |= (1 << i);
        }
    }
    return val;
}

int main() {
    Wire enable{LogicState::LOW};
    Bus<3> address;
    Bus<8> read_data;

    // Pre-loaded ROM image with 8 words of 8-bit data
    std::vector<std::size_t> rom_image = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
    };

    ROM<3, 8> rom(enable, address, read_data, rom_image);

    std::cout << "Testing ROM<3, 8> (8 words, 8 bits per word)...\n";

    // 1. Enable LOW -> read_data should output 0
    enable.write(LogicState::LOW);
    write_addr(address, 0);
    rom.evaluate();
    assert(read_data_val(read_data) == 0);
    std::cout << "[PASS] ROM disabled outputs 0\n";

    // 2. Enable HIGH -> read addresses 0 through 7
    enable.write(LogicState::HIGH);

    for (std::size_t i = 0; i < 8; ++i) {
        write_addr(address, static_cast<uint8_t>(i));
        rom.evaluate();
        uint8_t val = read_data_val(read_data);
        assert(val == rom_image[i]);
        std::cout << "[PASS] Address " << i << " reads 0x" << std::hex << (int)val << std::dec << "\n";
    }

    // 3. Dynamic content modification via set_word()
    rom.set_word(4, 0xAA);
    write_addr(address, 4);
    rom.evaluate();
    assert(read_data_val(read_data) == 0xAA);
    std::cout << "[PASS] Dynamic content update set_word(4, 0xAA) verified\n";

    std::cout << "All ROM tests passed successfully!\n";
    return 0;
}
