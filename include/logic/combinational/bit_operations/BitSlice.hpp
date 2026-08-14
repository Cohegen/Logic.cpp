#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

/**
 * @brief Bit Slice Component
 * Extracts a contiguous slice of `SliceWidth` bits starting at `StartBit` from `InWidth` bus onto `OutWidth` bus.
 */
template <std::size_t InWidth, std::size_t StartBit, std::size_t SliceWidth>
class BitSlice : public Component
{
    static_assert(StartBit + SliceWidth <= InWidth, "Bit slice bounds exceed input bus width.");

public:
    BitSlice(Bus<InWidth>& in_bus, Bus<SliceWidth>& out_bus)
        : m_inBus(in_bus), m_outBus(out_bus)
    {
    }

    void evaluate() noexcept override
    {
        for (std::size_t i = 0; i < SliceWidth; ++i)
        {
            m_outBus[i].write(m_inBus[StartBit + i].read());
        }
    }

private:
    Bus<InWidth>& m_inBus;
    Bus<SliceWidth>& m_outBus;
};

} // namespace logic
