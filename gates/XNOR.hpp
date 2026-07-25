/*
Implementation of an XNOR gate
It is true when both inputs have similar values
*/

#pragma once 
#include "Gates.hpp"
#include "Signals/wire.hpp"
#include "binary_gate.hpp"

namespace logic{
    class XnorGate:public BinaryGate
    {
           public:
               XnorGate(Wire& a,Wire& b,Wire& out):
                    BinaryGate(a,b,out){}

                void evaluate() override
                {
                    m_output.write(
                        logic_xnor(
                            m_inputA.read(),
                            m_inputB.read()
                        )
                    );
                }
    };
}