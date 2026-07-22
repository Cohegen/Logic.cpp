/*
Implementation of XOR gate
Output is true if and only if inputs have different values
*/

#pragma once
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic
{

class XORGate : public BinaryGate
{
public:
    XORGate(Wire& a, Wire& b, Wire& out)
        : BinaryGate(a, b, out)
    {
    }

    void evaluate() override
    {
        m_output.write(
            logic_xor(
                m_inputA.read(),
                m_inputB.read()));
    }
};

}
