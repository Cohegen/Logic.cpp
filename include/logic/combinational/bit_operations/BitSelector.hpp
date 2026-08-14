#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

/**
 * @brief Bit Selector Component
 * Extracts a single wire index `bit_index` from an input bus `in_bus` onto output `out_wire`.
 */
template <std::size_t Width>
class BitSelector : public Component
{
public:
    BitSelector(Bus<Width>& in_bus, Bus<Width>& bit_index, Wire& out_wire)
        : m_inBus(in_bus), m_bitIndex(bit_index), m_outWire(out_wire)
    {
    }

    void evaluate() noexcept override
    {
        std::size_t idx = 0;
        for (std::size_t i = 0; i < m_bitIndex.size(); ++i)
        {
            if (m_bitIndex[i].read() == LogicState::HIGH)
            {
                idx |= (static_cast<std::size_t>(1) << i);
            }
        }

        if (idx < Width)
        {
            m_outWire.write(m_inBus[idx].read());
        }
        else
        {
            m_outWire.write(LogicState::LOW);
        }
    }

private:
    Bus<Width>& m_inBus;
    Bus<Width>& m_bitIndex;
    Wire& m_outWire;
};

} // namespace logic
