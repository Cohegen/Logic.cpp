/*
    4-to-1 Multiplexer built using three 2-to-1 Multiplexers
*/
#pragma once

#include "signals/logicState.hpp"
#include "signals/wire.hpp"
#include "combinational/multiplexers/Mux2to1.hpp"
#include "simulator/Component.hpp"

namespace logic {

class Mux4to1: public Component {
public:
    Mux4to1(Wire& a, Wire& b, Wire& c, Wire& d, Wire& select0, Wire& select1, Wire& output)
        : m_mux0(a, b, select0, m_internal0),
          m_mux1(c, d, select0, m_internal1),
          m_mux2(m_internal0, m_internal1, select1, output) {}

    void evaluate()
    {
        m_mux0.evaluate();
        m_mux1.evaluate();
        m_mux2.evaluate();
    }

private:
    Wire m_internal0;
    Wire m_internal1;

    Mux2to1 m_mux0;
    Mux2to1 m_mux1;
    Mux2to1 m_mux2;
};

} // namespace logic
