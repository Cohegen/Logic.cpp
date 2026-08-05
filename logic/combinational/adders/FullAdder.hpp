/*
Implementation of a full adder
*/
#pragma once

#include "combinational/adders/HalfAdder.hpp"
#include "gates/OR.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

class FullAdder:public Component
{
public:
    FullAdder() = default;
    FullAdder(Wire& a,
              Wire& b,
              Wire& cin,
              Wire& sum,
              Wire& carry)
        : m_halfAdder1(a, b, m_sum1, m_carry1),
          m_halfAdder2(m_sum1, cin, sum, m_carry2),
          m_carryGate(m_carry1, m_carry2, carry)
    {
    }

    void evaluate() noexcept override
    {
        m_halfAdder1.evaluate();
        m_halfAdder2.evaluate();
        m_carryGate.evaluate();
    }

private:
    // Internal wires connecting the subcircuits
    Wire m_sum1;
    Wire m_carry1;
    Wire m_carry2;

    // Internal components
    HalfAdder m_halfAdder1;
    HalfAdder m_halfAdder2;
    OrGate m_carryGate;
};

} // namespace logic