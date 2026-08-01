/*
    SRLatch.hpp

    An implementation of an SR latch using two cross-coupled NOR gates.
*/

#pragma once

#include <cstddef>

#include "gates/NOR.hpp"
#include "signals/logicState.hpp"
#include "signals/wire.hpp"

namespace logic
{

class SRLatch
{
public:
    SRLatch(
        Wire& s,
        Wire& r,
        Wire& q,
        Wire& q_bar)
        : s_(s),
          r_(r),
          q_(q),
          q_bar_(q_bar),
          nor_gate1_(r_, q_bar_, q_),
          nor_gate2_(s_, q_, q_bar_)
    {
    }

    void evaluate() noexcept
    {
        for (std::size_t i = 0; i < MaxIterations; ++i)
        {
            const LogicState old_q = q_.read();
            const LogicState old_q_bar = q_bar_.read();

            nor_gate1_.evaluate();
            nor_gate2_.evaluate();

            if (old_q == q_.read() &&
                old_q_bar == q_bar_.read())
            {
                break;
            }
        }
    }

private:
    static constexpr std::size_t MaxIterations = 10;

    Wire& s_;
    Wire& r_;
    Wire& q_;
    Wire& q_bar_;

    NorGate nor_gate1_;
    NorGate nor_gate2_;
};

} // namespace logic
