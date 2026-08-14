#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

template <std::size_t InWidth, std::size_t OutWidth>
class ZeroExtender : public Component
{
    static_assert(OutWidth >= InWidth, "OutWidth must be greater than or equal to InWidth.");

public:
    ZeroExtender(Bus<InWidth>& in, Bus<OutWidth>& out)
        : m_in(in), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        for (std::size_t i = 0; i < InWidth; ++i)
        {
            m_out[i].write(m_in[i].read());
        }
        for (std::size_t i = InWidth; i < OutWidth; ++i)
        {
            m_out[i].write(LogicState::LOW);
        }
    }

private:
    Bus<InWidth>& m_in;
    Bus<OutWidth>& m_out;
};

} // namespace logic
