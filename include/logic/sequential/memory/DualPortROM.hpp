/*
    DualPortROM.hpp

    Dual-read-port Read-Only Memory (ROM) component template.
    Stores pre-loaded data in 2^AddressWidth words and exposes two
    independent combinational read ports gated by a shared enable signal.
*/

#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

#include "combinational/multiplexers/Mux.hpp"
#include "gates/NOT.hpp"
#include "signals/bus.hpp"
#include "signals/logicState.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic {

template <std::size_t AddressWidth, std::size_t DataWidth>
class DualPortROM : public Component {
    static_assert(AddressWidth > 0, "AddressWidth must be greater than zero.");
    static_assert(DataWidth > 0, "DataWidth must be greater than zero.");

public:
    static constexpr std::size_t NumWords = 1ULL << AddressWidth;

    DualPortROM(
        Wire& enable,
        Bus<AddressWidth>& address0,
        Bus<AddressWidth>& address1,
        Bus<DataWidth>& read_data0,
        Bus<DataWidth>& read_data1,
        const std::vector<std::size_t>& initial_contents = {}
    ) : enable_(enable),
        address0_(address0),
        address1_(address1),
        read_data0_(read_data0),
        read_data1_(read_data1),
        read_mux0_(selected_data0_, zero_bus0_, enable_inv_, read_data0_),
        read_mux1_(selected_data1_, zero_bus1_, enable_inv_, read_data1_)
    {
        contents_.fill(0);
        load_contents(initial_contents);
    }

    void load_contents(const std::vector<std::size_t>& contents) noexcept
    {
        const std::size_t count = std::min(contents.size(), NumWords);
        for (std::size_t i = 0; i < count; ++i) {
            contents_[i] = contents[i];
        }
    }

    void set_word(std::size_t index, std::size_t value) noexcept
    {
        if (index < NumWords) {
            contents_[index] = value;
        }
    }

    [[nodiscard]] std::size_t get_word(std::size_t index) const noexcept
    {
        return index < NumWords ? contents_[index] : 0;
    }

    void evaluate() noexcept override
    {
        const std::size_t address_val0 = get_address_index(address0_);
        const std::size_t address_val1 = get_address_index(address1_);
        const std::size_t val0 = contents_[address_val0];
        const std::size_t val1 = contents_[address_val1];

        for (std::size_t i = 0; i < DataWidth; ++i) {
            selected_data0_[i].write((val0 & (1ULL << i)) ? LogicState::HIGH : LogicState::LOW);
            selected_data1_[i].write((val1 & (1ULL << i)) ? LogicState::HIGH : LogicState::LOW);
        }

        enable_not_.evaluate();
        read_mux0_.evaluate();
        read_mux1_.evaluate();
    }

private:
    Wire& enable_;
    Bus<AddressWidth>& address0_;
    Bus<AddressWidth>& address1_;
    Bus<DataWidth>& read_data0_;
    Bus<DataWidth>& read_data1_;

    Wire enable_inv_;
    NotGate enable_not_{enable_, enable_inv_};

    Bus<DataWidth> zero_bus0_;
    Bus<DataWidth> zero_bus1_;
    Bus<DataWidth> selected_data0_;
    Bus<DataWidth> selected_data1_;
    std::array<std::size_t, NumWords> contents_;

    Mux<DataWidth> read_mux0_;
    Mux<DataWidth> read_mux1_;

    template <std::size_t Width>
    [[nodiscard]] static std::size_t get_address_index(const Bus<Width>& address) noexcept
    {
        std::size_t index = 0;
        for (std::size_t i = 0; i < Width; ++i) {
            if (address[i].read() == LogicState::HIGH) {
                index |= (1ULL << i);
            }
        }
        return index;
    }
};

} // namespace logic
