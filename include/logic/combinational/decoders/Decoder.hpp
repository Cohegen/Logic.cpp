/*
    Decoder.hpp

    A templated binary line decoder component.
    Decodes an N-bit binary address into a 1-hot activated output array of 2^N lines
    (or a custom OutLines <= 2^N lines).

    Truth Table (for 2:4 Decoder, Active-High Output):
    Enable (E) | A1  A0 | Y3  Y2  Y1  Y0
    -----------+--------+---------------
         0     |  X   X |  0   0   0   0
         1     |  0   0 |  0   0   0   1
         1     |  0   1 |  0   0   1   0
         1     |  1   0 |  0   1   0   0
         1     |  1   1 |  1   0   0   0
*/

#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "signals/logicState.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

/**
 * @brief Templated Binary Line Decoder
 * 
 * @tparam InBits Number of address input bits
 * @tparam OutLines Number of output lines (defaults to 2^InBits)
 */
template <std::size_t InBits, std::size_t OutLines = (static_cast<std::size_t>(1) << InBits)>
class Decoder : public Component
{
    static_assert(InBits > 0, "Decoder input bit-width must be greater than zero.");
    static_assert(InBits < (sizeof(std::size_t) * 8), "Decoder input bit-width is too large.");
    static_assert(OutLines > 0, "Decoder output line count must be greater than zero.");
    static_assert(OutLines <= (static_cast<std::size_t>(1) << InBits), "Decoder output lines cannot exceed 2^InBits.");

public:
    /// Constructor with Enable wire: Decoder(address, enable, outputs)
    Decoder(Bus<InBits>& in, Wire& enable, Bus<OutLines>& out)
        : m_in(in), m_enable(&enable), m_out(out)
    {
    }

    /// Constructor without Enable wire (always active)
    Decoder(Bus<InBits>& in, Bus<OutLines>& out)
        : m_in(in), m_enable(nullptr), m_out(out)
    {
    }

    /// Constructor with optional pointer to Enable wire: Decoder(address, enable_ptr, outputs)
    Decoder(Bus<InBits>& in, Wire* enable, Bus<OutLines>& out)
        : m_in(in), m_enable(enable), m_out(out)
    {
    }

    /// Constructor with Enable wire as third argument: Decoder(address, outputs, enable)
    Decoder(Bus<InBits>& in, Bus<OutLines>& out, Wire& enable)
        : m_in(in), m_enable(&enable), m_out(out)
    {
    }

    /// Constructor with optional pointer to Enable wire as third argument
    Decoder(Bus<InBits>& in, Bus<OutLines>& out, Wire* enable)
        : m_in(in), m_enable(enable), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        // If an enable wire is connected, check its state
        if (m_enable != nullptr)
        {
            LogicState en_state = m_enable->read();
            if (en_state == LogicState::LOW)
            {
                for (std::size_t i = 0; i < OutLines; ++i)
                {
                    m_out[i].write(LogicState::LOW);
                }
                return;
            }
            if (!is_binary(en_state))
            {
                for (std::size_t i = 0; i < OutLines; ++i)
                {
                    m_out[i].write(LogicState::UNKNOWN);
                }
                return;
            }
        }

        // Determine the active output line from input address bits
        std::size_t active_index = 0;
        for (std::size_t i = 0; i < InBits; ++i)
        {
            LogicState bit = m_in[i].read();
            if (!is_binary(bit))
            {
                for (std::size_t j = 0; j < OutLines; ++j)
                {
                    m_out[j].write(LogicState::UNKNOWN);
                }
                return;
            }
            if (bit == LogicState::HIGH)
            {
                active_index |= (static_cast<std::size_t>(1) << i);
            }
        }

        // 1-hot activated output lines
        for (std::size_t i = 0; i < OutLines; ++i)
        {
            m_out[i].write((i == active_index) ? LogicState::HIGH : LogicState::LOW);
        }
    }

private:
    Bus<InBits>& m_in;
    Wire* m_enable;
    Bus<OutLines>& m_out;
};

// Common type aliases
using Decoder1to2 = Decoder<1, 2>;
using Decoder2to4 = Decoder<2, 4>;
using Decoder3to8 = Decoder<3, 8>;
using Decoder4to16 = Decoder<4, 16>;

} // namespace logic
