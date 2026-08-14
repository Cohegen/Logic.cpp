/*
    N-bit Ripple Borrow Subtractor

    A Ripple Borrow Subtractor is composed of N Full Subtractors.
    Each Full Subtractor's borrow-out is connected to the borrow-in
    of the next Full Subtractor.

    
*/

#pragma once

#include <array>
#include <memory>

#include "combinational/subtractors/FullSubtractor.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

template <std::size_t N>
class RippleBorrowSubtractor:public Component
{
public:
    RippleBorrowSubtractor(std::array<Wire, N>& a,
                           std::array<Wire, N>& b,
                           Wire& borrowIn,
                           std::array<Wire, N>& difference,
                           Wire& borrowOut)
        : RippleBorrowSubtractor(a.data(), b.data(), borrowIn, difference.data(), borrowOut)
    {
    }

    RippleBorrowSubtractor(Bus<N>& a,
                           Bus<N>& b,
                           Wire& borrowIn,
                           Bus<N>& difference,
                           Wire& borrowOut)
        : RippleBorrowSubtractor(&a[0], &b[0], borrowIn, &difference[0], borrowOut)
    {
    }

    void evaluate() noexcept override
    {
        for (auto& subtractor : m_fullSubtractors)
        {
            if (subtractor)
            {
                subtractor->evaluate();
            }
        }
    }

private:
    RippleBorrowSubtractor(Wire* a,
                           Wire* b,
                           Wire& borrowIn,
                           Wire* difference,
                           Wire& borrowOut)
    {
        static_assert(N > 0, "RippleBorrowSubtractor must have at least one bit.");

        // Single-bit subtractor
        if constexpr (N == 1)
        {
            m_fullSubtractors[0] = std::make_unique<FullSubtractor>(
                a[0],
                b[0],
                borrowIn,
                difference[0],
                borrowOut);

            return;
        }

        // First Full Subtractor
        m_fullSubtractors[0] = std::make_unique<FullSubtractor>(
            a[0],
            b[0],
            borrowIn,
            difference[0],
            m_borrows[0]);

        // Middle Full Subtractors
        for (std::size_t i = 1; i < N - 1; ++i)
        {
            m_fullSubtractors[i] = std::make_unique<FullSubtractor>(
                a[i],
                b[i],
                m_borrows[i - 1],
                difference[i],
                m_borrows[i]);
        }

        // Last Full Subtractor
        m_fullSubtractors[N - 1] = std::make_unique<FullSubtractor>(
            a[N - 1],
            b[N - 1],
            m_borrows[N - 2],
            difference[N - 1],
            borrowOut);
    }
    // Internal borrow wires
    std::array<Wire, (N > 1 ? N - 1 : 0)> m_borrows;

    // One FullSubtractor per bit
    std::array<std::unique_ptr<FullSubtractor>, N> m_fullSubtractors;
};

}
