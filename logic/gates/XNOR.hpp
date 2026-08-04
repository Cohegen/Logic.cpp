/*
Implementation of an XNOR gate
It is true when both inputs have similar values
*/

#pragma once 
#include "gates/Gate.hpp"
#include "signals/wire.hpp
#include "simulator/Component.hpp"

namespace logic{
    class XnorGate:public Component
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