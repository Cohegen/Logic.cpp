#pragma once

#include <cstdint>

namespace logic {

class Clock
{
    public:

    void tick()
    {
        m_cycle++;

    }

    [[nodiscard]]
    std::uint64_t cycle() const 
    {
        return m_cycle;
    }

    private:

    std::uint64_t m_cycle{0};

};
}
