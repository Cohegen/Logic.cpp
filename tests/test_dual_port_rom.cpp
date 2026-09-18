#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include <logic/sequential/memory/DualPortROM.hpp>
#include <logic/signals/bus.hpp>
#include <logic/signals/logicState.hpp>
#include <logic/signals/wire.hpp>

using namespace logic;

void write_addr(Bus<3>& bus, uint8_t val)
{
    for (std::size_t i = 0; i < 3; ++i) {
        bus[i].write((val & (1 << i)) ? LogicState::HIGH : LogicState::LOW);
    }
}

uint8_t read_data_val(const Bus<8>& bus)
{
    uint8_t val = 0;
    for (std::size_t i = 0; i < 8; ++i) {
        if (bus[i].read() == LogicState::HIGH) {
            val |= (1 << i);
        }
    }
    return val;
}

int main()
{
    Wire enable{LogicState::LOW};
    Bus<3> address0;
    Bus<3> address1;
    Bus<8> read_data0;
    Bus<8> read_data1;

    std::vector<std::size_t> rom_image = {
        0x10, 0x21, 0x32, 0x43, 0x54, 0x65, 0x76, 0x87
    };

    DualPortROM<3, 8> rom(enable, address0, address1, read_data0, read_data1, rom_image);

    std::cout << "Testing DualPortROM<3, 8> (8 words, 8 bits per word)...\n";

    enable.write(LogicState::LOW);
    write_addr(address0, 1);
    write_addr(address1, 6);
    rom.evaluate();
    assert(read_data_val(read_data0) == 0);
    assert(read_data_val(read_data1) == 0);
    std::cout << "[PASS] DualPortROM disabled outputs 0 on both ports\n";

    enable.write(LogicState::HIGH);
    write_addr(address0, 2);
    write_addr(address1, 5);
    rom.evaluate();
    assert(read_data_val(read_data0) == rom_image[2]);
    assert(read_data_val(read_data1) == rom_image[5]);
    std::cout << "[PASS] Dual independent read ports verified\n";

    rom.set_word(7, 0xAA);
    write_addr(address0, 7);
    write_addr(address1, 0);
    rom.evaluate();
    assert(read_data_val(read_data0) == 0xAA);
    assert(read_data_val(read_data1) == rom_image[0]);
    assert(rom.get_word(7) == 0xAA);
    std::cout << "[PASS] Dynamic content update verified\n";

    std::cout << "All DualPortROM tests passed successfully!\n";
    return 0;
}
