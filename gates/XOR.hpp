/*
Implementation of XOR
It is also known as XOR,it is true if and only if
its inputs have different values otherwise false
*/

#pragma once 
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic{
    class XorGate:public BinaryGate
    {
        public:
            XorGate(Wire& a, Wire& b, Wire& out):
                 BinaryGate(a,b,out){}

            void evaluate()override
            {
                m_output.write(
                    logic_xor(
                        m_inputA.read(),
                        m_inputB.read()
                    )
                );
            }
    };
}
