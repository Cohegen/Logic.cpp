/*
NAND gate is the inverse of AND gate
*/

#pragma once
#include "gates/Gate.hpp"
#include "signals/wire.hpp"
#include "gates/binary_gate.hpp"
#include "simulator/Component.hpp"

namespace logic 
{
    class NandGate :public Component
    {
        public:
            NandGate(Wire& a,Wire& b,Wire& out):
                  BinaryGate(a,b,out){}

            void evaluate() override
            {
                m_output.write(
                    logic_nand(
                        m_inputA.read(),
                        m_inputB.read()
                    )
                );
            }
    };
}