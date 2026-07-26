/*
    2-to-1 Multiplexer

    Truth Table:
    Select | Output
    -------|-------
      0    | Input A
      1    | Input B
*/
#pragma once

#include "Signals/logicstate.hpp"
#include "Signals/wire.hpp"

namespace logic {

class Mux2to1 {
public:
    Mux2to1(Wire& a, Wire& b, Wire& select, Wire& output)
        : m_inputA(a), m_inputB(b), m_select(select), m_output(output) {}

    void evaluate()
    {
        const LogicState selectState = m_select.read();
        if (selectState == LogicState::LOW)
        {
            m_output.write(m_inputA.read());
        }
        else if (selectState == LogicState::HIGH)
        {
            m_output.write(m_inputB.read());
        }
        else
        {
            m_output.write(LogicState::UNKNOWN);
        }
    }

private:
    Wire& m_inputA;
    Wire& m_inputB;
    Wire& m_select;
    Wire& m_output;
};

} 
