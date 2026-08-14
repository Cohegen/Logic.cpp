/*

    This file defines the central simulation engine for Logic.cpp.

    The simulator is responsible for coordinating the evaluation
    of all registered hardware components and advancing simulation
    time.

    The simulator does NOT own the registered components.
    It only stores pointers to them. The caller is responsible
    for ensuring the components remain valid for the lifetime
    of the simulator.
*/

#pragma once

#include <cstdint>
#include <vector>

#include "Component.hpp"
#include "signals/clock.hpp"

namespace logic
{

class Simulator
{
public:
    Simulator() = default;
    ~Simulator() = default;

    // Prevent copying the constrcutor and copy operator
    Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;

    // Allow moving.
    Simulator(Simulator&&) = default;
    Simulator& operator=(Simulator&&) = default;

    /*
        Registers a hardware component with the simulator.

        Ownership is NOT transferred.
    */
    void add_component(Component& component);

    /*
        Advances the simulation by one clock cycle.

        The implementation will:
            1. Advance the clock.
            2. Evaluate all registered components.
            3. (Later) Handle sequential updates and events.
    */
    void tick();

    /*
        Resets the simulator.

        The initial implementation simply resets the clock.
        Future versions may also clear pending events and
        scheduled updates.
    */
    void reset() noexcept;

    /*
        Returns the current simulation cycle.
    */
    [[nodiscard]]
    std::uint64_t cycle() const noexcept;

    /*
        Returns a const reference to the simulator clock.
    */
    [[nodiscard]]
    const Clock& clock() const noexcept;

private:
    Clock clock_;

    // Non-owning pointers to registered components.
    std::vector<Component*> components_;
};

}