/*
NAND gate is the inverse of AND gate
*/

#pragma once
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic 
{
    class NandGate :public BinaryGate
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