/*
    Component.hpp

    Base interface for every hardware component in the Logic.cpp
    simulation framework.

    All digital components (gates, multiplexers, adders, latches,
    flip-flops, registers, ALUs) derive from this class and
    implement evaluate(), allowing the simulator to evaluate
    heterogeneous hardware through a common interface.
*/

#pragma once

namespace logic
{

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    // Prevent copying through the base interface.
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    // Allow moving if needed.
    Component(Component&&) = default;
    Component& operator=(Component&&) = default;

    /*
        Evaluates the component.

        For combinational circuits this propagates signals through the
        component.

        For sequential circuits this computes or propagates the
        component's next state according to the current simulator model.
    */
    virtual void evaluate() noexcept = 0;
};

}