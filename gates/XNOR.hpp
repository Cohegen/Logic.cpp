/*
Implementation of XNOR gate
Output is true when both inputs have identical values
*/

#pragma once
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic
{

class XNORGate : public BinaryGate
{
public:
    XNORGate(Wire& a, Wire& b, Wire& out)
        : BinaryGate(a, b, out)
    {
    }

    void evaluate() override
    {
        m_output.write(
            logic_xnor(
                m_inputA.read(),
                m_inputB.read()));
    }
};

}
