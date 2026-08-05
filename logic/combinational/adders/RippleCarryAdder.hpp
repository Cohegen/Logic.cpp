/*
    N-bit Ripple Carry Adder

    A Ripple Carry Adder is composed of N Full Adders.
    Each Full Adder's carry-out is connected to the carry-in
    of the next Full Adder.

    
*/

#pragma once

#include <array>
#include <memory>

#include "combinational/adders/FullAdder.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

template <std::size_t N>
class RippleCarryAdder:public Component
{
public:
    RippleCarryAdder(std::array<Wire, N>& a,
                     std::array<Wire, N>& b,
                     Wire& carryIn,
                     std::array<Wire, N>& sum,
                     Wire& carryOut)
        : RippleCarryAdder(a.data(), b.data(), carryIn, sum.data(), carryOut)
    {
    }

    RippleCarryAdder(Bus<N>& a,
                     Bus<N>& b,
                     Wire& carryIn,
                     Bus<N>& sum,
                     Wire& carryOut)
        : RippleCarryAdder(&a[0], &b[0], carryIn, &sum[0], carryOut)
    {
    }

    void evaluate() noexcept override
    {
        for (auto& adder : m_fullAdders)
        {
            if (adder)
            {
                adder->evaluate();
            }
        }
    }

private:
    RippleCarryAdder(Wire* a,
                     Wire* b,
                     Wire& carryIn,
                     Wire* sum,
                     Wire& carryOut)
    {
        static_assert(N > 0, "RippleCarryAdder must have at least one bit.");

        // Single-bit adder
        if constexpr (N == 1)
        {
            m_fullAdders[0] = std::make_unique<FullAdder>(
                a[0],
                b[0],
                carryIn,
                sum[0],
                carryOut);

            return;
        }

        // First Full Adder
        m_fullAdders[0] = std::make_unique<FullAdder>(
            a[0],
            b[0],
            carryIn,
            sum[0],
            m_carries[0]);

        // Middle Full Adders
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            m_fullAdders[i] = std::make_unique<FullAdder>(
                a[i],
                b[i],
                m_carries[i - 1],
                sum[i],
                m_carries[i]);
        }

        // Last Full Adder
        m_fullAdders[N - 1] = std::make_unique<FullAdder>(
            a[N - 1],
            b[N - 1],
            m_carries[N - 2],
            sum[N - 1],
            carryOut);
    }
    // Internal carry wires
    std::array<Wire, (N > 1 ? N - 1 : 0)> m_carries;

    // One FullAdder per bit
    std::array<std::unique_ptr<FullAdder>, N> m_fullAdders;
};

} 