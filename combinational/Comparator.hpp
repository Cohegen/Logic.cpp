/*
    Magnitude Comparator (1-Bit and N-Bit template)
    Built using gate-level components and Bus<N> signals.

    Outputs:
    - Equal   : HIGH if A == B
    - Greater : HIGH if A > B
    - Less    : HIGH if A < B
*/
#pragma once

#include <cstddef>
#include <vector>
#include <memory>
#include "Signals/wire.hpp"
#include "Signals/bus.hpp"
#include "gates/AND.hpp"
#include "gates/NOT.hpp"
#include "gates/OR.hpp"
#include "gates/XNOR.hpp"

namespace logic {

/// 1-Bit Magnitude Comparator
class Comparator1Bit {
public:
    Comparator1Bit(Wire& a, Wire& b, Wire& equal, Wire& greater, Wire& less)
        : m_notA(a, m_invA),
          m_notB(b, m_invB),
          m_xnorGate(a, b, equal),
          m_andGreater(a, m_invB, greater),
          m_andLess(m_invA, b, less) {}

    void evaluate()
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

/// N-Bit Magnitude Comparator using Bus<N>
template <std::size_t N>
class Comparator {
    static_assert(N > 0, "Comparator bit-width N must be greater than zero.");

public:
    Comparator(Bus<N>& busA, Bus<N>& busB, Wire& equal, Wire& greater, Wire& less)
        : m_busA(busA),
          m_busB(busB),
          m_equalOut(equal),
          m_greaterOut(greater),
          m_lessOut(less)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            m_bitComps.push_back(
                std::make_unique<Comparator1Bit>(
                    m_busA[i], m_busB[i], m_bitEqual[i], m_bitGreater[i], m_bitLess[i]
                )
            );
        }
    }

    void evaluate()
    {
        // 1. Evaluate per-bit comparators
        for (auto& comp : m_bitComps)
        {
            comp->evaluate();
        }

        // 2. Compute Equal Output (A == B when all bitEqual bits are HIGH)
        LogicState eqState = LogicState::HIGH;
        for (std::size_t i = 0; i < N; ++i)
        {
            eqState = logic_and(eqState, m_bitEqual[i].read());
        }
        m_equalOut.write(eqState);

        // 3. Compute Greater Output (A > B from MSB down to LSB)
        LogicState gtState = LogicState::LOW;
        for (std::size_t i = N; i > 0; --i)
        {
            std::size_t idx = i - 1;
            LogicState term = m_bitGreater[idx].read();
            for (std::size_t j = idx + 1; j < N; ++j)
            {
                term = logic_and(term, m_bitEqual[j].read());
            }
            gtState = logic_or(gtState, term);
        }
        m_greaterOut.write(gtState);

        // 4. Compute Less Output (A < B from MSB down to LSB)
        LogicState ltState = LogicState::LOW;
        for (std::size_t i = N; i > 0; --i)
        {
            std::size_t idx = i - 1;
            LogicState term = m_bitLess[idx].read();
            for (std::size_t j = idx + 1; j < N; ++j)
            {
                term = logic_and(term, m_bitEqual[j].read());
            }
            ltState = logic_or(ltState, term);
        }
        m_lessOut.write(ltState);
    }

private:
    Bus<N>& m_busA;
    Bus<N>& m_busB;
    Wire& m_equalOut;
    Wire& m_greaterOut;
    Wire& m_lessOut;

    Bus<N> m_bitEqual;
    Bus<N> m_bitGreater;
    Bus<N> m_bitLess;
    std::vector<std::unique_ptr<Comparator1Bit>> m_bitComps;
};

} // namespace logic
