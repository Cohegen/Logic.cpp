/*
    DLatch.hpp

    An implementation of a D latch built from a NOT gate and a gated SR latch.
*/

#pragma once

#include "gates/NOT.hpp"
#include "sequential/latches/GatedSRLatch.hpp"
#include "signals/wire.hpp"

namespace logic
{

class DLatch
{
public:
    DLatch(
        Wire& d,
        Wire& enable,
        Wire& q,
        Wire& q_bar)
        : d_(d),
          enable_(enable),
          q_(q),
          q_bar_(q_bar),
          not_gate_(d_, not_d_),
          gated_sr_latch_(d_, not_d_, enable_, q_, q_bar_)
    {
    }

    void evaluate() noexcept
    {
        not_gate_.evaluate();
        gated_sr_latch_.evaluate();
    }

private:
    Wire& d_;
    Wire& enable_;
    Wire& q_;
    Wire& q_bar_;

    Wire not_d_;

    NotGate not_gate_;
    GatedSRLatch gated_sr_latch_;
};

} 
