#ifndef CLA4_HPP
#define CLA4_HPP

#include "Gates.h"
#include <bitset>
#include <cstddef>

class CLA4
{
private:
    std::bitset<4> a;
    std::bitset<4> b;
    bool carryIn{false};

public:
    CLA4(const std::bitset<4>& lhs, const std::bitset<4>& rhs, bool cin = false)
        : a(lhs), b(rhs), carryIn(cin) {}

    std::bitset<4> sum() const
    {
        const auto carries = carryBits();
        std::bitset<4> result;

        for (std::size_t i = 0; i < 4; ++i) {
            result[i] = Gates{a[i], b[i], carries[i]}.XOR();
        }

        return result;
    }

    bool carryOut() const
    {
        return carryBits()[4];
    }

    bool groupPropagate() const
    {
        return propagate().all();
    }

    bool groupGenerate() const
    {
        const auto p = propagate();
        const auto g = generate();

        return g[3]
            || (p[3] && g[2])
            || (p[3] && p[2] && g[1])
            || (p[3] && p[2] && p[1] && g[0]);
    }

    std::bitset<5> carryBits() const
    {
        const auto p = propagate();
        const auto g = generate();

        std::bitset<5> carry;
        carry[0] = carryIn;
        carry[1] = g[0] || (p[0] && carry[0]);
        carry[2] = g[1] || (p[1] && g[0]) || (p[1] && p[0] && carry[0]);
        carry[3] = g[2] || (p[2] && g[1]) || (p[2] && p[1] && g[0])
            || (p[2] && p[1] && p[0] && carry[0]);
        carry[4] = groupGenerate() || (groupPropagate() && carry[0]);
        return carry;
    }

private:
    std::bitset<4> propagate() const
    {
        return a ^ b;
    }

    std::bitset<4> generate() const
    {
        return a & b;
    }
};

#endif
