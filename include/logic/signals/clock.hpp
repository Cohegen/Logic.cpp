/*
    Clock.hpp

    A digital clock used to drive sequential circuits.
*/

#pragma once

#include <cstdint>

#include "logic/signals/logicState.hpp"

namespace logic
{

class Clock
{
public:
    Clock() = default;

    /// Advances the clock by one half-cycle.
    void tick() noexcept
    {
        previous_state_ = current_state_;

        current_state_ =
            (current_state_ == LogicState::LOW)
                ? LogicState::HIGH
                : LogicState::LOW;

        ++cycle_;
    }

    /// Resets the clock to its initial state.
    void reset() noexcept
    {
        cycle_ = 0;
        previous_state_ = LogicState::LOW;
        current_state_ = LogicState::LOW;
    }

    [[nodiscard]]
    std::uint64_t cycle() const noexcept
    {
        return cycle_;
    }

    [[nodiscard]]
    LogicState state() const noexcept
    {
        return current_state_;
    }

    [[nodiscard]]
    bool is_high() const noexcept
    {
        return current_state_ == LogicState::HIGH;
    }

    [[nodiscard]]
    bool is_low() const noexcept
    {
        return current_state_ == LogicState::LOW;
    }

    [[nodiscard]]
    bool is_rising_edge() const noexcept
    {
        return previous_state_ == LogicState::LOW &&
               current_state_ == LogicState::HIGH;
    }

    [[nodiscard]]
    bool is_falling_edge() const noexcept
    {
        return previous_state_ == LogicState::HIGH &&
               current_state_ == LogicState::LOW;
    }

private:
    std::uint64_t cycle_{0};

    LogicState previous_state_{LogicState::LOW};
    LogicState current_state_{LogicState::LOW};
};

} 
