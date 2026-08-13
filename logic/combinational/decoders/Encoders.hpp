#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

/**
 * @brief 4-to-2 Binary Encoder
 * Encodes 4 one-hot input lines into a 2-bit binary output code.
 */
class Encoder4to2 : public Component
{
public:
    Encoder4to2(Bus<4>& in, Bus<2>& out, Wire& valid)
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

/**
 * @brief Parameterized Priority Encoder
 * Encodes N input lines to OutBits = ceil(log2(N)), returning the code of the highest active input line.
 */
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
