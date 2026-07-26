/*
    2-to-4 Decoder

    Truth Table

    A   B   | Y0 Y1 Y2 Y3
    ----------------------
    0   0   | 1  0  0  0
    0   1   | 0  1  0  0
    1   0   | 0  0  1  0
    1   1   | 0  0  0  1
*/

#pragma once

#include "Signals/wire.hpp"
#include "Signals/logicstate.hpp"

namespace logic
{

class Dec2to4
{
public:
    Dec2to4(
        Wire& a,
        Wire& b,
        Wire& out0,
        Wire& out1,
        Wire& out2,
        Wire& out3)
        : m_inputA(a),
          m_inputB(b),
          m_output0(out0),
          m_output1(out1),
          m_output2(out2),
          m_output3(out3)
    {
    }

    void evaluate()
    {
        const LogicState a = m_inputA.read();
        const LogicState b = m_inputB.read();

        // Unknown input propagates to all outputs
        if (a == LogicState::UNKNOWN ||
            b == LogicState::UNKNOWN)
        {
            m_output0.write(LogicState::UNKNOWN);
            m_output1.write(LogicState::UNKNOWN);
            m_output2.write(LogicState::UNKNOWN);
            m_output3.write(LogicState::UNKNOWN);
            return;
        }

        // Defaulting all outputs LOW
        m_output0.write(LogicState::LOW);
        m_output1.write(LogicState::LOW);
        m_output2.write(LogicState::LOW);
        m_output3.write(LogicState::LOW);

        // Activating exactly one output
        if (a == LogicState::LOW &&
            b == LogicState::LOW)
        {
            m_output0.write(LogicState::HIGH);
        }
        else if (a == LogicState::LOW &&
                 b == LogicState::HIGH)
        {
            m_output1.write(LogicState::HIGH);
        }
        else if (a == LogicState::HIGH &&
                 b == LogicState::LOW)
        {
            m_output2.write(LogicState::HIGH);
        }
        else
        {
            // A == HIGH && B == HIGH
            m_output3.write(LogicState::HIGH);
        }
    }

private:
    Wire& m_inputA;
    Wire& m_inputB;

    Wire& m_output0;
    Wire& m_output1;
    Wire& m_output2;
    Wire& m_output3;
};

} 
