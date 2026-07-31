/*
    Full Subtractor implementation

    Computes:

        Difference = A - B - Bin

    Outputs:

        Difference
        Borrow
*/

#pragma once

#include "signals/wire.hpp"
#include "combinational/subtractors/HalfSubtractor.hpp"
#include "gates/OR.hpp"

namespace logic
{

class FullSubtractor
{
public:

    FullSubtractor(
        Wire& a,
        Wire& b,
        Wire& bin,
        Wire& difference,
        Wire& borrow
    )
        :
        m_sub1(a, b, diff1_, borrow1_),
        m_sub2(diff1_, bin, difference, borrow2_),
        or_gate(borrow1_, borrow2_, borrow)
    {
    }


    void evaluate()
    {
        m_sub1.evaluate();
        m_sub2.evaluate();
        or_gate.evaluate();
    }


private:

    Wire diff1_;

    Wire borrow1_;

    Wire borrow2_;


    HalfSubtractor m_sub1;

    HalfSubtractor m_sub2;

    OrGate or_gate;
};

} 