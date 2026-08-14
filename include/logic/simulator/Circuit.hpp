#pragma once

#include <vector>

#include "Component.hpp"

namespace logic
{

class Circuit
{
public:
    Circuit() = default;
    virtual ~Circuit() = default;

    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;

    Circuit(Circuit&&) = default;
    Circuit& operator=(Circuit&&) = default;

    /*
        Creates and initializes the hardware that
        belongs to this circuit
    */
    virtual void initialize() = 0;

    /*
        Connects the components together
    */
    virtual void connect() = 0;

    /*
        Returns the components that belong to
        this circuit.
    */
    [[nodiscard]]
    virtual const std::vector<Component*>& components() const noexcept = 0;
};

}