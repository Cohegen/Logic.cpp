#pragma once

#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "gates/AND.hpp"
#include "gates/NOT.hpp"
#include "gates/XNOR.hpp"
#include "simulator/Component.hpp"

namespace logic {

/// 1-Bit Magnitude Comparator
class Comparator1Bit:public Component {
public:
    Comparator1Bit(Wire& a, Wire& b, Wire& equal, Wire& greater, Wire& less)
        : m_notA(a, m_invA),
          m_notB(b, m_invB),
          m_xnorGate(a, b, equal),
          m_andGreater(a, m_invB, greater),
          m_andLess(m_invA, b, less) {}

    void evaluate() noexcept override
    {
        m_notA.evaluate();
        m_notB.evaluate();
        m_xnorGate.evaluate();
        m_andGreater.evaluate();
        m_andLess.evaluate();
    }

private:
    Wire m_invA;
    Wire m_invB;

    NotGate  m_notA;
    NotGate  m_notB;
    XnorGate m_xnorGate;
    ANDGate  m_andGreater;
    ANDGate  m_andLess;
};

} // namespace logic
