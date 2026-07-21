

#pragma once

#include "signal.hpp"

namespace logic
{

class Wire
{
public:
    Wire() = default;

    explicit Wire(LogicState state)
        : m_signal(state)
    {
    }

    /// Writes a logic state onto the wire.
    void write(LogicState state)
    {
        m_signal.set(state);
    }

    /// Reads the current logic state carried by the wire.
    [[nodiscard]]
    LogicState read() const
    {
        return m_signal.value();
    }

private:
    Signal m_signal;
};

} 
