/*
    Memory.hpp

    Unified Memory Interface System Component.
    Integrates MemoryController, ROM (lower memory space), and RAM (upper memory space).
*/

#pragma once

#include <cstddef>
#include <vector>

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "signals/clock.hpp"
#include "signals/logicState.hpp"
#include "simulator/Component.hpp"
#include "sequential/memory/RAM.hpp"
#include "sequential/memory/ROM.hpp"
#include "sequential/memory/MemoryController.hpp"

namespace logic {

template <std::size_t AddressWidth, std::size_t DataWidth>
class Memory : public Component {
    static_assert(AddressWidth > 1, "Memory system AddressWidth must be at least 2.");
    static_assert(DataWidth > 0, "DataWidth must be greater than zero.");

public:
    static constexpr std::size_t SubAddressWidth = AddressWidth - 1;

    Memory(
        Clock& clock,
        Wire& reset,
        Wire& request,
        Wire& command, // 0 = Read, 1 = Write
        Wire& ready,
        Bus<AddressWidth>& address,
        Bus<DataWidth>& write_data,
        Bus<DataWidth>& read_data,
        const std::vector<std::size_t>& rom_init_contents = {}
    ) : clock_(clock),
        reset_(reset),
        request_(request),
        command_(command),
        ready_(ready),
        address_(address),
        write_data_(write_data),
        read_data_(read_data),
        controller_(
            request_, command_, address_, write_data_, read_data_, ready_,
            rom_cs_, ram_cs_, mem_read_en_, mem_write_en_,
            mem_address_, mem_write_data_, mem_read_data_
        ),
        rom_(rom_read_en_, rom_address_, rom_read_data_, rom_init_contents),
        ram_(clock_, reset_, ram_read_en_, ram_write_en_, ram_address_, mem_write_data_, ram_read_data_)
    {
    }

    /// Load or overwrite ROM contents
    void load_rom(const std::vector<std::size_t>& contents) noexcept
    {
        rom_.load_contents(contents);
    }

    void evaluate() noexcept override
    {
        // 1. Evaluate Controller first to propagate address, request, command -> mem_address, rom_cs, ram_cs, mem_read_en, mem_write_en
        controller_.evaluate();

        // 2. Slice lower address bits for sub-components
        for (std::size_t i = 0; i < SubAddressWidth; ++i) {
            rom_address_[i].write(mem_address_[i].read());
            ram_address_[i].write(mem_address_[i].read());
        }

        // 3. Gate sub-component read/write enables using chip select signals
        rom_read_en_.write((rom_cs_.read() == LogicState::HIGH && mem_read_en_.read() == LogicState::HIGH)
                           ? LogicState::HIGH : LogicState::LOW);

        ram_read_en_.write((ram_cs_.read() == LogicState::HIGH && mem_read_en_.read() == LogicState::HIGH)
                           ? LogicState::HIGH : LogicState::LOW);

        ram_write_en_.write((ram_cs_.read() == LogicState::HIGH && mem_write_en_.read() == LogicState::HIGH)
                            ? LogicState::HIGH : LogicState::LOW);

        // 4. Evaluate ROM & RAM modules
        rom_.evaluate();
        ram_.evaluate();

        // 5. Mux read data back to memory controller
        for (std::size_t j = 0; j < DataWidth; ++j) {
            if (rom_cs_.read() == LogicState::HIGH) {
                mem_read_data_[j].write(rom_read_data_[j].read());
            } else if (ram_cs_.read() == LogicState::HIGH) {
                mem_read_data_[j].write(ram_read_data_[j].read());
            } else {
                mem_read_data_[j].write(LogicState::LOW);
            }
        }

        // 6. Re-evaluate controller to drive final read_data bus
        controller_.evaluate();
    }

private:
    Clock& clock_;
    Wire& reset_;
    Wire& request_;
    Wire& command_;
    Wire& ready_;

    Bus<AddressWidth>& address_;
    Bus<DataWidth>& write_data_;
    Bus<DataWidth>& read_data_;

    // Internal interconnect signals
    Wire rom_cs_;
    Wire ram_cs_;
    Wire mem_read_en_;
    Wire mem_write_en_;
    Bus<AddressWidth> mem_address_;
    Bus<DataWidth> mem_write_data_;
    Bus<DataWidth> mem_read_data_;

    Wire rom_read_en_;
    Wire ram_read_en_;
    Wire ram_write_en_;
    Bus<SubAddressWidth> rom_address_;
    Bus<SubAddressWidth> ram_address_;
    Bus<DataWidth> rom_read_data_;
    Bus<DataWidth> ram_read_data_;

    // Internal sub-components
    MemoryController<AddressWidth, DataWidth> controller_;
    ROM<SubAddressWidth, DataWidth> rom_;
    RAM<SubAddressWidth, DataWidth> ram_;
};

} // namespace logic
