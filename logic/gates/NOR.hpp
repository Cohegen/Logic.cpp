/*
Implementation of NOR gate
NOR gate is the inverse of OR gate
*/

#pragma once
#include "gates/Gate.hpp"
#include "signals/wire.hpp"
#include "gates/binary_gate.hpp"
#include "simulator/Component.hpp"

namespace logic 
{
    class NorGate : public Component 
    {
    public:
        NorGate(Wire& a, Wire& b, Wire& out)
            : BinaryGate(a, b, out) {}

        void evaluate() override 
        {
            m_output.write(
                logic_nor(
                    m_inputA.read(),
                    m_inputB.read()
                )
            );
        }
    };
}