/*
    Half Subtractor

    Inputs:
        A
        B

    Outputs:
        Difference
        Borrow

    Truth Table

    A   B   | Difference Borrow
    ----------------------------
    0   0   |     0        0
    0   1   |     1        1
    1   0   |     1        0
    1   1   |     0        0
*/

#pragma once

#include "gates/XOR.hpp"
#include "gates/NOT.hpp"
#include "gates/AND.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

class HalfSubtractor:public Component
{
public:

    HalfSubtractor(
        Wire& a,
        Wire& b,
        Wire& difference,
        Wire& borrow
    )
        :
        not_gate(a, not_a_),
        difference_gate(a, b, difference),
        borrow_gate(not_a_, b, borrow)
    {
    }


    void evaluate() noexcept override
    {
        // Generate NOT A
        not_gate.evaluate();

        // Difference = A XOR B
        difference_gate.evaluate();

        // Borrow = NOT A AND B
        borrow_gate.evaluate();
    }


private:

    Wire not_a_;

    NotGate not_gate;

    XorGate difference_gate;

    ANDGate borrow_gate;
};

} 
