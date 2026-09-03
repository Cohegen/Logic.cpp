/*
    3-to-1 Multiplexer built using two 2-to-1 Multiplexers

    Select encoding:
        select1 select0 | Output
        --------------|-------
            0    0    | Input A
            0    1    | Input B
            1    0    | Input C
            1    1    | Input C (unused)
*/
#pragma once

#include "signals/wire.hpp"
#include "combinational/multiplexers/Mux2to1.hpp"
#include "simulator/Component.hpp"

namespace logic {

class Mux3to1 : public Component {
public:
    Mux3to1(Wire& a, Wire& b, Wire& c, Wire& select0, Wire& select1, Wire& output)
        : m_mux0(a, b, select0, m_internal0),
          m_mux1(m_internal0, c, select1, output) {}

    void evaluate() noexcept override
    {
        m_mux0.evaluate();
        m_mux1.evaluate();
    }

private:
    Wire m_internal0;

    Mux2to1 m_mux0;
    Mux2to1 m_mux1;
};

} // namespace logic
