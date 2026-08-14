#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

template <std::size_t InLines, std::size_t OutBits>
class PriorityEncoder : public Component
{
public:
    PriorityEncoder(Bus<InLines>& in, Bus<OutBits>& out, Wire& valid)
        : m_in(in), m_out(out), m_valid(valid)
    {
    }

    void evaluate() noexcept override
    {
        bool has_active = false;
        std::size_t highest_active = 0;

        for (std::size_t i = InLines; i > 0; --i)
        {
            std::size_t idx = i - 1;
            if (m_in[idx].read() == LogicState::HIGH)
            {
                has_active = true;
                highest_active = idx;
                break;
            }
        }

        m_valid.write(has_active ? LogicState::HIGH : LogicState::LOW);

        for (std::size_t b = 0; b < OutBits; ++b)
        {
            if (has_active && ((highest_active >> b) & 1))
            {
                m_out[b].write(LogicState::HIGH);
            }
            else
            {
                m_out[b].write(LogicState::LOW);
            }
        }
    }

private:
    Bus<InLines>& m_in;
    Bus<OutBits>& m_out;
    Wire& m_valid;
};

} // namespace logic
