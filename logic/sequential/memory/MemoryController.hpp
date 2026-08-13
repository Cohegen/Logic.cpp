/*
    MemoryController.hpp

    Memory bus controller managing request arbitration, memory mapping address
    decoding (RAM vs ROM region selection), control signal routing, and ready handshakes.
*/

#pragma once

#include <cstddef>
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "signals/logicState.hpp"
#include "simulator/Component.hpp"
#include "gates/AND.hpp"
#include "gates/NOT.hpp"

namespace logic {

template <std::size_t AddressWidth, std::size_t DataWidth>
class MemoryController : public Component {
    static_assert(AddressWidth > 0, "AddressWidth must be greater than zero.");
    static_assert(DataWidth > 0, "DataWidth must be greater than zero.");

public:
    MemoryController(
        Wire& request,
        Wire& command, // LOW = Read, HIGH = Write
        Bus<AddressWidth>& address,
        Bus<DataWidth>& write_data,
        Bus<DataWidth>& read_data,
        Wire& ready,
        Wire& rom_cs,
        Wire& ram_cs,
        Wire& mem_read_en,
        Wire& mem_write_en,
        Bus<AddressWidth>& mem_address,
        Bus<DataWidth>& mem_write_data,
        Bus<DataWidth>& mem_read_data
    ) : request_(request),
        command_(command),
        address_(address),
        write_data_(write_data),
        read_data_(read_data),
        ready_(ready),
        rom_cs_(rom_cs),
        ram_cs_(ram_cs),
        mem_read_en_(mem_read_en),
        mem_write_en_(mem_write_en),
        mem_address_(mem_address),
        mem_write_data_(mem_write_data),
        mem_read_data_(mem_read_data)
    {
    }

    void evaluate() noexcept override
    {
        // 1. Pass-through address and write data buses to memory side
        for (std::size_t i = 0; i < AddressWidth; ++i) {
            mem_address_[i].write(address_[i].read());
        }
        for (std::size_t j = 0; j < DataWidth; ++j) {
            mem_write_data_[j].write(write_data_[j].read());
        }

        // 2. Decode Memory Map based on MSB of address bus
        // Address MSB == LOW  -> ROM Region
        // Address MSB == HIGH -> RAM Region
        bool is_request = (request_.read() == LogicState::HIGH);
        bool is_write = (command_.read() == LogicState::HIGH);
        bool msb_set = (address_[AddressWidth - 1].read() == LogicState::HIGH);

        LogicState rom_sel = (is_request && !msb_set) ? LogicState::HIGH : LogicState::LOW;
        LogicState ram_sel = (is_request && msb_set) ? LogicState::HIGH : LogicState::LOW;

        rom_cs_.write(rom_sel);
        ram_cs_.write(ram_sel);

        // 3. Control signals
        LogicState read_en = (is_request && !is_write) ? LogicState::HIGH : LogicState::LOW;
        LogicState write_en = (is_request && is_write && msb_set) ? LogicState::HIGH : LogicState::LOW;

        mem_read_en_.write(read_en);
        mem_write_en_.write(write_en);

        // 4. Data read pass-through & ready handshake logic
        ready_.write(is_request ? LogicState::HIGH : LogicState::LOW);

        for (std::size_t j = 0; j < DataWidth; ++j) {
            read_data_[j].write(read_en == LogicState::HIGH ? mem_read_data_[j].read() : LogicState::LOW);
        }
    }

private:
    // CPU Master side
    Wire& request_;
    Wire& command_;
    Bus<AddressWidth>& address_;
    Bus<DataWidth>& write_data_;
    Bus<DataWidth>& read_data_;
    Wire& ready_;

    // Memory Device side
    Wire& rom_cs_;
    Wire& ram_cs_;
    Wire& mem_read_en_;
    Wire& mem_write_en_;
    Bus<AddressWidth>& mem_address_;
    Bus<DataWidth>& mem_write_data_;
    Bus<DataWidth>& mem_read_data_;
};

} // namespace logic
