/*
Modelling a signal--
From digital design we know that a signal can be either high or low on a wire
*/

#pragma once

#include "logicstate.hpp"

namespace logic
{

class Signal
{
public:
    Signal() = default;

    explicit Signal(LogicState state)
        : m_state(state)
    {
    }

    [[nodiscard]]
    LogicState value() const
    {
        return m_state;
    }

    void set(LogicState state)
    {
        m_state = state;
    }

private:
    LogicState m_state{LogicState::LOW};
};

} // namespace logic
