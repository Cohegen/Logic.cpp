/*
Implementation of OR GATE.
It's false when both of its inputs are false
otherwise false
*/

#pragma once
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic
{

class OrGate : public BinaryGate
{
public:
    OrGate(Wire& a, Wire& b, Wire& out)
        : BinaryGate(a, b, out)
    {
    }

    void evaluate() override
    {
        m_output.write(
            logic_or(
                m_inputA.read(),
                m_inputB.read()));
    }
};

} 