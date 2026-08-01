/*
    GatedSRLatch.hpp

    An implementation of a gated SR latch.
*/

#pragma once

#include "gates/AND.hpp"
#include "sequential/latches/SRLatch.hpp"
#include "signals/wire.hpp"

namespace logic
{

class GatedSRLatch
{
public:
    GatedSRLatch(
        Wire& s,
        Wire& r,
        Wire& enable,
        Wire& q,
        Wire& q_bar)
        : s_(s),
          r_(r),
          enable_(enable),
          q_(q),
          q_bar_(q_bar),
          and_gate1_(s_, enable_, gated_s_),
          and_gate2_(r_, enable_, gated_r_),
          sr_latch_(gated_s_, gated_r_, q_, q_bar_)
    {
    }

    void evaluate() noexcept
    {
        and_gate1_.evaluate();
        and_gate2_.evaluate();
        sr_latch_.evaluate();
    }

private:
    Wire& s_;
    Wire& r_;
    Wire& enable_;
    Wire& q_;
    Wire& q_bar_;

    Wire gated_s_;
    Wire gated_r_;

    ANDGate and_gate1_;
    ANDGate and_gate2_;
    SRLatch sr_latch_;
};

} // namespace logic
