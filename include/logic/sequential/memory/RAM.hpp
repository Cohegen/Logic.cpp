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
#include "gates/NOT.hpp"
#include "combinational/multiplexers/Mux.hpp"

namespace logic {

template <std::size_t AddressWidth, std::size_t DataWidth>
class RAM : public Component {
    static_assert(AddressWidth > 0, "AddressWidth must be greater than zero.");
    static_assert(DataWidth > 0, "DataWidth must be greater than zero.");

public:
    static constexpr std::size_t NumWords = 1ULL << AddressWidth;

    RAM(
        Clock& clock,
        Wire& reset,
        Wire& read_enable,
        Wire& write_enable,
        Bus<AddressWidth>& address,
        Bus<DataWidth>& write_data,
        Bus<DataWidth>& read_data
    ) : clock_(clock),
        reset_(reset),
        read_enable_(read_enable),
        write_enable_(write_enable),
        address_(address),
        write_data_(write_data),
        read_data_(read_data),
        read_mux_(selected_data_, zero_bus_, read_enable_inv_, read_data_)
    {
        registers_.reserve(NumWords);
        reg_inputs_.resize(NumWords);
        memory_words_.resize(NumWords);
        word_write_enables_.resize(NumWords);
        gated_clocks_.resize(NumWords);
        write_enable_ands_.reserve(NumWords);
        clock_gating_ands_.reserve(NumWords);
        reset_muxes_.reserve(NumWords);

        // Instantiate per-word write enable logic, gated clocks, and registers
        for (std::size_t i = 0; i < NumWords; ++i) {
            // Write enable AND gate: word_write_enables_[i] = write_enable_ AND word_select_[i]
            write_enable_ands_.emplace_back(
                write_enable_,
                word_select_[i],
                word_write_enables_[i]
            );

            // Clock gating AND gate: gated_clocks_[i] = clock_wire_ AND word_write_enables_[i]
            clock_gating_ands_.emplace_back(
                clock_wire_,
                word_write_enables_[i],
                gated_clocks_[i]
            );

            // Mux instance per word for resetting or feeding write_data_ into register inputs
            reset_muxes_.emplace_back(
                write_data_,
                zero_bus_,
                reset_,
                reg_inputs_[i]
            );

            // Register instance for word i
            registers_.emplace_back(
                reg_inputs_[i],
                gated_clocks_[i],
                memory_words_[i]
            );
        }
    }

    void evaluate() noexcept override
    {
        clock_wire_.write(clock_.state());

        // 1. Decode address bus into 1-of-N word select signals
        decode_address();

        // 2. Evaluate reset multiplexers and write enable gating per word
        for (std::size_t i = 0; i < NumWords; ++i) {
            write_enable_ands_[i].evaluate();

            // When reset is active (HIGH), force write enable so registers reset to 0
            if (reset_.read() == LogicState::HIGH) {
                word_write_enables_[i].write(LogicState::HIGH);
            }

            clock_gating_ands_[i].evaluate();
            reset_muxes_[i].evaluate();
            registers_[i].evaluate();
        }

        // 3. Select memory word data based on decoded address
        std::size_t selected_index = get_address_index();
        for (std::size_t j = 0; j < DataWidth; ++j) {
            selected_data_[j].write(memory_words_[selected_index][j].read());
        }

        // 4. Output read data gated by read_enable via Mux<DataWidth>
        read_enable_not_.evaluate();
        read_mux_.evaluate();
    }

private:
    // Inputs & Outputs
    Clock& clock_;
    Wire& reset_;
    Wire& read_enable_;
    Wire& write_enable_;
    Bus<AddressWidth>& address_;
    Bus<DataWidth>& write_data_;
    Bus<DataWidth>& read_data_;

    // Internal signals
    Wire clock_wire_;
    Wire read_enable_inv_;
    NotGate read_enable_not_{read_enable_, read_enable_inv_};

    std::array<Wire, NumWords> word_select_;
    std::vector<Wire> word_write_enables_;
    std::vector<Wire> gated_clocks_;

    Bus<DataWidth> zero_bus_;
    std::vector<Bus<DataWidth>> reg_inputs_;
    std::vector<Bus<DataWidth>> memory_words_;
    Bus<DataWidth> selected_data_;

    // Gates & Sub-components
    std::vector<ANDGate> write_enable_ands_;
    std::vector<ANDGate> clock_gating_ands_;
    std::vector<Mux<DataWidth>> reset_muxes_;
    std::vector<Register<DataWidth>> registers_;
    Mux<DataWidth> read_mux_;

    // Helper: decode binary address bus into one-hot word_select_
    void decode_address() noexcept
    {
        std::size_t addr_val = get_address_index();
        for (std::size_t i = 0; i < NumWords; ++i) {
            word_select_[i].write(i == addr_val ? LogicState::HIGH : LogicState::LOW);
        }
    }

    // Helper: convert address bus lines to size_t integer index
    [[nodiscard]] std::size_t get_address_index() const noexcept
    {
        std::size_t index = 0;
        for (std::size_t i = 0; i < AddressWidth; ++i) {
            if (address_[i].read() == LogicState::HIGH) {
                index |= (1ULL << i);
            }
        }
        return index;
    }
};

} // namespace logic