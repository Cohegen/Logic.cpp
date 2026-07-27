/*
    2-to-4 Decoder

    Truth Table

    A   B   | Y0 Y1 Y2 Y3
    ----------------------
    0   0   | 1  0  0  0
    0   1   | 0  1  0  0
    1   0   | 0  0  1  0
    1   1   | 0  0  0  1

    Implemented hierarchically using:
        - 2 NOT gates
        - 4 AND gates

    Boolean Equations

    Y0 = ¬A ∧ ¬B
    Y1 = ¬A ∧  B
    Y2 =  A ∧ ¬B
    Y3 =  A ∧  B
*/

#pragma once

#include "Signals/wire.hpp"
#include "Signals/logicstate.hpp"
#include "gates/NOT.hpp"
#include "gates/AND.hpp"

namespace logic
{

class Dec2to4
{
public:

    Dec2to4(
        Wire& a,
        Wire& b,
        Wire& out0,
        Wire& out1,
        Wire& out2,
        Wire& out3)
        :
        // Internal NOT stage
        m_notGateA(a, m_notA),
        m_notGateB(b, m_notB),

        // AND stage
        m_and0(m_notA, m_notB, out0),
        m_and1(m_notA, b,      out1),
        m_and2(a,      m_notB, out2),
        m_and3(a,      b,      out3)
    {
    }

    void evaluate()
    {
        // Generate inverted signals
        m_notGateA.evaluate();
        m_notGateB.evaluate();

        // Generate outputs
        m_and0.evaluate();
        m_and1.evaluate();
        m_and2.evaluate();
        m_and3.evaluate();
    }

private:

    // Internal wires
    Wire m_notA;
    Wire m_notB;

    // Internal gates
    NotGate m_notGateA;
    NotGate m_notGateB;

    ANDGate m_and0;
    ANDGate m_and1;
    ANDGate m_and2;
    ANDGate m_and3;
};

} // namespace logic
