/*
    SimulationContext.hpp

    Stores simulation-wide state shared between the
    simulator and all registered hardware components.

    Every component evaluated by the simulator receives
    access to the same SimulationContext, allowing it
    to query simulation-wide information without owning
    or managing the simulator directly.

    Future versions will extend this class with support
    for event scheduling, dirty signal tracking, and
    multiple clock domains.
*/

#pragma once

#include <cstdint>

namespace logic
{

class Simulator;

class SimulationContext
{
public:
    explicit SimulationContext(Simulator& simulator);

    [[nodiscard]]
    Simulator& simulator() noexcept;

    [[nodiscard]]
    const Simulator& simulator() const noexcept;

    [[nodiscard]]
    std::uint64_t cycle() const noexcept;

private:
    Simulator* simulator_;
};

}