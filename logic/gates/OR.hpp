/*
Implementation of OR GATE.
It's false when both of its inputs are false
otherwise false
*/

#pragma once
#include "gates/Gate.hpp"
#include "signals/wire.hpp"
#include "gates/binary_gate.hpp"
#include "simulator/Component.hpp"
namespace logic
{

class OrGate : public Component
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