#pragma once

#include <ostream>
#include <stdexcept>

namespace logic
{
    enum class LogicState{
        LOW=0,
        HIGH,
        UNKNOWN,
        HIGH_IMPEDANCE
    };

    [[nodiscard]]
    constexpr char to_char(LogicState state)
    {
        switch(state)
        {
            case LogicState::LOW:
                   return '0';

            case LogicState::HIGH:
                   return '1';

            case LogicState::UNKNOWN:
                  return 'X';

            case LogicState::HIGH_IMPEDANCE:
                   return 'Z';
        }

        throw std::logic_error("Invalid LogicState");
    }

    [[nodiscard]]
    constexpr LogicState from_bool(bool value)
    {
        return value ? LogicState::HIGH: LogicState::LOW;
    }

    //checking whether state is in binary state
    [[nodiscard]]
    constexpr bool is_binary(LogicState state)
    {
        return state == LogicState::LOW || state == LogicState::HIGH;
    }

    //turns state into its respective boolean value
    [[nodiscard]]
    constexpr bool to_bool(LogicState state)
    {
        if(!is_binary(state))
             throw std::logic_error("LogicState is not binary!");

        return state == LogicState::HIGH;
    }

    //inverter
    [[nodiscard]]
    constexpr LogicState logic_not(LogicState state)
    {
        switch (state) {
            case LogicState::LOW:
                return LogicState::HIGH;

            case LogicState::HIGH:
                return LogicState::LOW;

            case LogicState::UNKNOWN:
                  return LogicState::UNKNOWN;

            case LogicState::HIGH_IMPEDANCE:
                   return LogicState::UNKNOWN;
        }

        return LogicState::UNKNOWN;
    }

    //AND helper
    [[nodiscard]]
    constexpr LogicState logic_and(LogicState state1, LogicState state2)
    {
    // 0 AND anything = 0
    if (state1 == LogicState::LOW ||
        state2 == LogicState::LOW)
    {
        return LogicState::LOW;
    }

    // 1 AND 1 = 1
    if (state1 == LogicState::HIGH &&
        state2 == LogicState::HIGH)
    {
        return LogicState::HIGH;
    }

    // Any remaining case involves UNKNOWN or HIGH_IMPEDANCE.
    return LogicState::UNKNOWN;
    }

    [[nodiscard]]
    constexpr LogicState logic_or(LogicState state1, LogicState state2)
    {
        // 1 OR anything = 1
        if (state1 == LogicState::HIGH ||
            state2 == LogicState::HIGH)
        {
            return LogicState::HIGH;
        }
    
        // 0 OR 0 = 0
        if (state1 == LogicState::LOW &&
            state2 == LogicState::LOW)
        {
            return LogicState::LOW;
        }
    
        // Any remaining case involves UNKNOWN or HIGH_IMPEDANCE.
        return LogicState::UNKNOWN;
    }

    

    inline std::ostream& operator<<(std::ostream& os, LogicState state)
    {
        return os << to_char(state);
    }
}
