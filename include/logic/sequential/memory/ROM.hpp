/*
    ROM.hpp

    Read-Only Memory (ROM) component template.
    Stores pre-loaded binary data/firmware array of 2^AddressWidth words.
*/

#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include <initializer_list>

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "signals/logicState.hpp"
#include "simulator/Component.hpp"
#include "gates/NOT.hpp"
#include "combinational/multiplexers/Mux.hpp"

namespace logic {

template <std::size_t AddressWidth, std::size_t DataWidth>
class ROM : public Component {
    static_assert(AddressWidth > 0, "AddressWidth must be greater than zero.");
    static_assert(DataWidth > 0, "DataWidth must be greater than zero.");

public:
    static constexpr std::size_t NumWords = 1ULL << AddressWidth;

    /// Constructor initializing ROM with optional pre-loaded initial content array
    ROM(
        Wire& enable,
        Bus<AddressWidth>& address,
        Bus<DataWidth>& read_data,
        const std::vector<std::size_t>& initial_contents = {}
    ) : enable_(enable),
        address_(address),
        read_data_(read_data),
        read_mux_(selected_data_, zero_bus_, enable_inv_, read_data_)
    {
        contents_.fill(0);
        load_contents(initial_contents);
    }

    /// Load or overwrite ROM binary contents
    void load_contents(const std::vector<std::size_t>& contents) noexcept
    {
        std::size_t count = std::min(contents.size(), NumWords);
        for (std::size_t i = 0; i < count; ++i) {
            contents_[i] = contents[i];
        }
    }

    /// Set word value at specific address index
    void set_word(std::size_t index, std::size_t val) noexcept
    {
        if (index < NumWords) {
            contents_[index] = val;
        }
    }

    /// Get word value at specific address index
    [[nodiscard]] std::size_t get_word(std::size_t index) const noexcept
    {
        return index < NumWords ? contents_[index] : 0;
    }

    void evaluate() noexcept override
    {
        // Compute address index from address bus
        std::size_t addr_val = get_address_index();

        // Read stored word value at index into selected_data_ bus
        std::size_t val = contents_[addr_val];
        for (std::size_t j = 0; j < DataWidth; ++j) {
            selected_data_[j].write((val & (1ULL << j)) ? LogicState::HIGH : LogicState::LOW);
        }

        // Enable gating via bus-level Mux
        enable_not_.evaluate();
        read_mux_.evaluate();
    }

private:
    Wire& enable_;
    Bus<AddressWidth>& address_;
    Bus<DataWidth>& read_data_;

    Wire enable_inv_;
    NotGate enable_not_{enable_, enable_inv_};

    Bus<DataWidth> zero_bus_;
    Bus<DataWidth> selected_data_;
    std::array<std::size_t, NumWords> contents_;
    Mux<DataWidth> read_mux_;

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
