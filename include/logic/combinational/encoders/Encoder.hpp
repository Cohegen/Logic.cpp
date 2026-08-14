#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

class Encoder : public Component
{
public:
    Encoder(Bus<4>& in, Bus<2>& out, Wire& valid)
        : m_in(in), m_out(out), m_valid(valid)
    {
    }

    void evaluate() noexcept override
    {
        bool has_active = false;
        std::size_t active_index = 0;

        for (std::size_t i = 0; i < 4; ++i)
        {
            if (m_in[i].read() == LogicState::HIGH)
            {
                has_active = true;
                active_index = i;
                break;
            }
        }

        m_valid.write(has_active ? LogicState::HIGH : LogicState::LOW);

        if (has_active)
        {
            m_out[0].write((active_index & 1) ? LogicState::HIGH : LogicState::LOW);
            m_out[1].write((active_index & 2) ? LogicState::HIGH : LogicState::LOW);
        }
        else
        {
            m_out[0].write(LogicState::LOW);
            m_out[1].write(LogicState::LOW);
        }
    }

private:
    Bus<4>& m_in;
    Bus<2>& m_out;
    Wire& m_valid;
};

using Encoder4to2 = Encoder;

} // namespace logic
