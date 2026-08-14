#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

/**
 * @brief Tri-State Buffer
 * Drives `output` with `input` state when `enable` is HIGH. Drives HIGH_IMPEDANCE (Z) when enable is LOW.
 */
class TriStateBuffer : public Component
{
public:
    TriStateBuffer(Wire& input, Wire& enable, Wire& output)
        : m_input(input), m_enable(enable), m_output(output)
    {
    }

    void evaluate() noexcept override
    {
        if (m_enable.read() == LogicState::HIGH)
        {
            m_output.write(m_input.read());
        }
        else
        {
            m_output.write(LogicState::HIGH_IMPEDANCE);
        }
    }

private:
    Wire& m_input;
    Wire& m_enable;
    Wire& m_output;
};

/**
 * @brief Bus Driver / Controlled Bus Driver
 * Drives a target shared bus `out_bus` with `in_bus` when `enable` control wire is HIGH.
 * Drives all wires in `out_bus` to HIGH_IMPEDANCE (Z) when `enable` is LOW.
 */
template <std::size_t Width>
class BusDriver : public Component
{
public:
    BusDriver(Bus<Width>& in_bus, Wire& enable, Bus<Width>& out_bus)
        : m_inBus(in_bus), m_enable(enable), m_outBus(out_bus)
    {
    }

    void evaluate() noexcept override
    {
        if (m_enable.read() == LogicState::HIGH)
        {
            for (std::size_t i = 0; i < Width; ++i)
            {
                m_outBus[i].write(m_inBus[i].read());
            }
        }
        else
        {
            for (std::size_t i = 0; i < Width; ++i)
            {
                m_outBus[i].write(LogicState::HIGH_IMPEDANCE);
            }
        }
    }

private:
    Bus<Width>& m_inBus;
    Wire& m_enable;
    Bus<Width>& m_outBus;
};

} // namespace logic
