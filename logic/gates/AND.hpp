/*
Implementation of AND gate
In AND gate, the result is true only if
both inputs are TRUE
*/

#pragma  once
#include "gates/Gate.hpp"
#include "signals/wire.hpp"
#include "gates/binary_gate.hpp"
#include "simulator/Component.hpp"

namespace logic{
    class ANDGate:public Component
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