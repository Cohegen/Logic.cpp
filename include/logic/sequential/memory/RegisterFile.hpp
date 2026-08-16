/*
    RegisterFile.hpp

    Dual-read-port, single-write-port Register File component.
    Contains 2^AddressWidth registers of width DataWidth.
*/

#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include <utility>

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "signals/clock.hpp"
#include "signals/logicState.hpp"
#include "simulator/Component.hpp"
#include "sequential/registers/register.hpp"
#include "gates/AND.hpp"
#include "combinational/multiplexers/Mux.hpp"

namespace logic {

template <std::size_t AddressWidth, std::size_t DataWidth>
class RegisterFile : public Component {
    static_assert(AddressWidth > 0, "AddressWidth must be greater than zero.");
    static_assert(DataWidth > 0, "DataWidth must be greater than zero.");

public:
    static constexpr std::size_t NumRegisters = 1ULL << AddressWidth;

    RegisterFile(
        Clock& clock,
        Wire& reset,
        Wire& write_enable,
        Bus<AddressWidth>& read_addr1,
        Bus<AddressWidth>& read_addr2,
        Bus<AddressWidth>& write_addr,
        Bus<DataWidth>& write_data,
        Bus<DataWidth>& read_data1,
        Bus<DataWidth>& read_data2
    ) : clock_(clock),
        reset_(reset),
        write_enable_(write_enable),
        read_addr1_(read_addr1),
        read_addr2_(read_addr2),
        write_addr_(write_addr),
        write_data_(write_data),
        read_data1_(read_data1),
        read_data2_(read_data2)
    {
        registers_.reserve(NumRegisters);
        reg_inputs_.resize(NumRegisters);
        reg_outputs_.resize(NumRegisters);
        word_write_enables_.resize(NumRegisters);
        gated_clocks_.resize(NumRegisters);
        write_enable_ands_.reserve(NumRegisters);
        clock_gating_ands_.reserve(NumRegisters);
        reset_muxes_.reserve(NumRegisters);

        // Instantiate per-register write gating, reset muxing, and registers
        for (std::size_t i = 0; i < NumRegisters; ++i) {
            // Write enable AND: word_write_enables_[i] = write_enable_ AND write_select_[i]
            write_enable_ands_.emplace_back(
                write_enable_,
                write_select_[i],
                word_write_enables_[i]
            );

            // Clock gating AND: gated_clocks_[i] = clock_wire_ AND word_write_enables_[i]
            clock_gating_ands_.emplace_back(
                clock_wire_,
                word_write_enables_[i],
                gated_clocks_[i]
            );

            // Mux instance per register for reset / write data routing
            reset_muxes_.emplace_back(
                write_data_,
                zero_bus_,
                reset_,
                reg_inputs_[i]
            );

            // Register instance for register i
            registers_.emplace_back(
                reg_inputs_[i],
                gated_clocks_[i],
                reg_outputs_[i]
            );
        }
    }

    void evaluate() noexcept override
    {
        clock_wire_.write(clock_.state());

        // 1. Decode write address bus into one-hot write_select_ signals
        std::size_t w_idx = get_bus_index(write_addr_);
        for (std::size_t i = 0; i < NumRegisters; ++i) {
            write_select_[i].write(i == w_idx ? LogicState::HIGH : LogicState::LOW);
        }

        // 2. Evaluate write enable logic, reset muxing, and register storage
        for (std::size_t i = 0; i < NumRegisters; ++i) {
            write_enable_ands_[i].evaluate();

            if (reset_.read() == LogicState::HIGH) {
                word_write_enables_[i].write(LogicState::HIGH);
            }

            clock_gating_ands_[i].evaluate();
            reset_muxes_[i].evaluate();
            registers_[i].evaluate();
        }

        // 3. Multi-port combinational read evaluation
        std::size_t r1_idx = get_bus_index(read_addr1_);
        std::size_t r2_idx = get_bus_index(read_addr2_);

        for (std::size_t j = 0; j < DataWidth; ++j) {
            read_data1_[j].write(reg_outputs_[r1_idx][j].read());
            read_data2_[j].write(reg_outputs_[r2_idx][j].read());
        }
    }

private:
    Clock& clock_;
    Wire& reset_;
    Wire& write_enable_;

    Bus<AddressWidth>& read_addr1_;
    Bus<AddressWidth>& read_addr2_;
    Bus<AddressWidth>& write_addr_;
    Bus<DataWidth>& write_data_;
    Bus<DataWidth>& read_data1_;
    Bus<DataWidth>& read_data2_;

    Wire clock_wire_;

    std::array<Wire, NumRegisters> write_select_;
    std::vector<Wire> word_write_enables_;
    std::vector<Wire> gated_clocks_;

    Bus<DataWidth> zero_bus_;
    std::vector<Bus<DataWidth>> reg_inputs_;
    std::vector<Bus<DataWidth>> reg_outputs_;

    std::vector<ANDGate> write_enable_ands_;
    std::vector<ANDGate> clock_gating_ands_;
    std::vector<Mux<DataWidth>> reset_muxes_;
    std::vector<Register<DataWidth>> registers_;

    template <std::size_t Width>
    [[nodiscard]] std::size_t get_bus_index(const Bus<Width>& bus) const noexcept
    {
        std::size_t index = 0;
        for (std::size_t i = 0; i < Width; ++i) {
            if (bus[i].read() == LogicState::HIGH) {
                index |= (1ULL << i);
            }
        }
        return index;
    }
};

} // namespace logic
