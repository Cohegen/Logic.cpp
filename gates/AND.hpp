/*
Implementation of AND gate
In AND gate, the result is true only if
both inputs are TRUE
*/

#pragma  once
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic{
    class ANDGate:public BinaryGate
    {
         public:
            ANDGate(Wire& a, Wire& b, Wire& out):
                 BinaryGate(a,b,out){}

            void evaluate() override
            {
                m_output.write(
                    logic_and(m_inputA.read(),m_inputB.read())
                );
            }
    };
}