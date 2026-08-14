#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

template <std::size_t Width>
class LogicalLeftShifter : public Component
{
public:
    LogicalLeftShifter(Bus<Width>& in, Bus<Width>& shift_amount, Bus<Width>& out)
        : m_in(in), m_shiftAmount(shift_amount), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        std::size_t amount = 0;
        std::size_t shift_bus_size = m_shiftAmount.size();
        for (std::size_t i = 0; i < shift_bus_size; ++i)
        {
            if (m_shiftAmount[i].read() == LogicState::HIGH)
            {
                amount |= (static_cast<std::size_t>(1) << i);
            }
        }

        for (std::size_t i = 0; i < Width; ++i)
        {
            if (i >= amount)
            {
                m_out[i].write(m_in[i - amount].read());
            }
            else
            {
                m_out[i].write(LogicState::LOW);
            }
        }
    }

private:
    Bus<Width>& m_in;
    Bus<Width>& m_shiftAmount;
    Bus<Width>& m_out;
};

} // namespace logic
