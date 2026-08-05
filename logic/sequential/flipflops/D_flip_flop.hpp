/*
    DFlipFlop.hpp

    An implementation of a master-slave D Flip-Flop.

    The flip-flop is constructed hierarchically from:

        • One NotGate
        • Two DLatch objects

    The master latch is enabled when the clock is LOW.
    The slave latch is enabled when the clock is HIGH.

    Together they form an edge-triggered storage element.
*/

#pragma once

#include "signals/wire.hpp"

#include "gates/NOT.hpp"
#include "sequential/latches/DLatch.hpp"
#include "simulator/Component.hpp"

namespace logic
{

class DFlipFlop:public Component
{
public:
    DFlipFlop(
        Wire& d,
        Wire& clock,
        Wire& q,
        Wire& q_bar)
        : d_(d),
          clock_(clock),
          q_(q),
          q_bar_(q_bar),

          // Generate the inverted clock.
          not_gate_(clock_, clock_not_),

          // Master latch is enabled when clock is LOW.
          master_latch_(
              d_,
              clock_not_,
              master_q_,
              master_q_bar_
          ),

          // Slave latch is enabled when clock is HIGH.
          slave_latch_(
              master_q_,
              clock_,
              q_,
              q_bar_
          )
    {
    }

    [[nodiscard]]
    Wire& q() noexcept
    {
        return q_;
    }

    [[nodiscard]]
    Wire& q_bar() noexcept
    {
        return q_bar_;
    }

    void evaluate() noexcept
    {
        // Generate the inverted clock.
        not_gate_.evaluate();

        // Update the master latch.
        master_latch_.evaluate();

        // Update the slave latch.
        slave_latch_.evaluate();
    }

private:
    
    // Inputs
    

    Wire& d_;
    Wire& clock_;

    
    // Outputs

    Wire& q_;
    Wire& q_bar_;

    
    // Internal wires

    // Inverted clock used by the master latch.
    Wire clock_not_;

    // Outputs of the master latch.
    Wire master_q_;
    Wire master_q_bar_;

    
    // Hardware implementation

    NotGate not_gate_;

    DLatch master_latch_;
    DLatch slave_latch_;
};

} // namespace logic