/*
Implementation of NOR gate
NOR gate is the inverse of OR gate
*/

#pragma once
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic 
{
    class NorGate : public BinaryGate 
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