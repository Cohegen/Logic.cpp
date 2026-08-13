#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

template <std::size_t Width, std::size_t ShiftBits>
class BarrelShifter : public Component
{
public:
    BarrelShifter(Bus<Width>& in, Bus<ShiftBits>& shift_amount, Bus<2>& mode, Bus<Width>& out)
        : m_in(in), m_shiftAmount(shift_amount), m_mode(mode), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        std::size_t amount = 0;
        for (std::size_t i = 0; i < ShiftBits; ++i)
        {
            if (m_shiftAmount[i].read() == LogicState::HIGH)
            {
                amount |= (static_cast<std::size_t>(1) << i);
            }
        }
        amount %= Width;

        std::size_t mode_val = 0;
        if (m_mode[0].read() == LogicState::HIGH) mode_val |= 1;
        if (m_mode[1].read() == LogicState::HIGH) mode_val |= 2;

        LogicState sign_bit = m_in[Width - 1].read();

        for (std::size_t i = 0; i < Width; ++i)
        {
            switch (mode_val)
            {
            case 0: // Logical Left Shift
                if (i >= amount)
                    m_out[i].write(m_in[i - amount].read());
                else
                    m_out[i].write(LogicState::LOW);
                break;

            case 1: // Logical Right Shift
                if (i + amount < Width)
                    m_out[i].write(m_in[i + amount].read());
                else
                    m_out[i].write(LogicState::LOW);
                break;

            case 2: // Arithmetic Right Shift
                if (i + amount < Width)
                    m_out[i].write(m_in[i + amount].read());
                else
                    m_out[i].write(sign_bit);
                break;

            case 3: // Rotate Right
                m_out[i].write(m_in[(i + amount) % Width].read());
                break;
            }
        }
    }

private:
    Bus<Width>& m_in;
    Bus<ShiftBits>& m_shiftAmount;
    Bus<2>& m_mode;
    Bus<Width>& m_out;
};

} // namespace logic
