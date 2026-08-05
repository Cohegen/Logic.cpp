/*
    2-to-1 Multiplexer built using gate-level components (NOT, AND, OR gates)

    Truth Table:
    Select | Output
    -------|-------
      0    | Input A
      1    | Input B
*/
#pragma once

#include "signals/wire.hpp"
#include "gates/AND.hpp"
#include "gates/NOT.hpp"
#include "gates/OR.hpp"
#include "simulator/Component.hpp"

namespace logic {

class Mux2to1: public Component {
public:
    Mux2to1(Wire& a, Wire& b, Wire& select, Wire& output)
        : m_notGate(select, m_selectInv),
          m_andGateA(a, m_selectInv, m_termA),
          m_andGateB(b, select, m_termB),
          m_orGate(m_termA, m_termB, output) {}

    void evaluate() noexcept override
    {
        m_notGate.evaluate();
        m_andGateA.evaluate();
        m_andGateB.evaluate();
        m_orGate.evaluate();
    }

private:
    Wire m_selectInv;
    Wire m_termA;
    Wire m_termB;

    NotGate m_notGate;
    ANDGate m_andGateA;
    ANDGate m_andGateB;
    OrGate  m_orGate;
};

} 

