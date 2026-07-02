#ifndef SUBTRACTOR_HPP
#define SUBTRACTOR_HPP

#include "ripple_carry_adder.hpp"
#include <bitset>
#include <cstddef>

template <std::size_t N>
class Subtractor
{
private:
    std::bitset<N> a;
    std::bitset<N> b;

public:
    Subtractor(const std::bitset<N>& minuend, const std::bitset<N>& subtrahend)
        : a(minuend), b(subtrahend) {}

    std::bitset<N> difference() const
    {
        const RippleCarryAdder<N> adder(a, ~b, true);
        return adder.sum();
    }

    bool borrowOut() const
    {
        const RippleCarryAdder<N> adder(a, ~b, true);
        return !adder.carryOut();
    }
};

#endif
