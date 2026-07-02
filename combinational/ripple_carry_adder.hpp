#ifndef RIPPLE_CARRY_ADDER_HPP
#define RIPPLE_CARRY_ADDER_HPP

#include "fullAdder.hpp"
#include <bitset>
#include <cstddef>
#include <utility>

template <std::size_t N>
class RippleCarryAdder
{
private:
    std::bitset<N> a;
    std::bitset<N> b;
    bool carryIn{false};

public:
    RippleCarryAdder(const std::bitset<N>& lhs, const std::bitset<N>& rhs, bool cin = false)
        : a(lhs), b(rhs), carryIn(cin) {}

    std::bitset<N> sum() const
    {
        auto result = add();
        return result.first;
    }

    bool carryOut() const
    {
        auto result = add();
        return result.second;
    }

private:
    std::pair<std::bitset<N>, bool> add() const
    {
        std::bitset<N> result;
        bool carry = carryIn;

        for (std::size_t i = 0; i < N; ++i) {
            const FullAdder adder{a[i], b[i], carry};
            result[i] = adder.Sum();
            carry = adder.Carry();
        }

        return {result, carry};
    }
};

#endif
