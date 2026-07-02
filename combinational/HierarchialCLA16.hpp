#ifndef HIERARCHIAL_CLA16_HPP
#define HIERARCHIAL_CLA16_HPP

#include "CLA4.hpp"
#include <bitset>

class HierarchialCLA16
{
private:
    std::bitset<16> a;
    std::bitset<16> b;
    bool carryIn{false};

public:
    HierarchialCLA16(const std::bitset<16>& lhs, const std::bitset<16>& rhs, bool cin = false)
        : a(lhs), b(rhs), carryIn(cin) {}

    std::bitset<16> sum() const
    {
        std::bitset<16> result;
        auto carries = blockCarries();

        for (std::size_t block = 0; block < 4; ++block) {
            const CLA4 cla4(blockBits(a, block), blockBits(b, block), carries[block]);
            const auto partial = cla4.sum();

            for (std::size_t bit = 0; bit < 4; ++bit) {
                result[(block * 4) + bit] = partial[bit];
            }
        }

        return result;
    }

    bool carryOut() const
    {
        return blockCarries()[4];
    }

    std::bitset<5> blockCarries() const
    {
        std::bitset<4> p;
        std::bitset<4> g;

        for (std::size_t block = 0; block < 4; ++block) {
            const CLA4 cla4(blockBits(a, block), blockBits(b, block), false);
            p[block] = cla4.groupPropagate();
            g[block] = cla4.groupGenerate();
        }

        std::bitset<5> carry;
        carry[0] = carryIn;
        carry[1] = g[0] || (p[0] && carry[0]);
        carry[2] = g[1] || (p[1] && g[0]) || (p[1] && p[0] && carry[0]);
        carry[3] = g[2] || (p[2] && g[1]) || (p[2] && p[1] && g[0])
            || (p[2] && p[1] && p[0] && carry[0]);
        carry[4] = g[3] || (p[3] && g[2]) || (p[3] && p[2] && g[1])
            || (p[3] && p[2] && p[1] && g[0])
            || (p[3] && p[2] && p[1] && p[0] && carry[0]);
        return carry;
    }

private:
    static std::bitset<4> blockBits(const std::bitset<16>& value, std::size_t block)
    {
        std::bitset<4> result;
        for (std::size_t bit = 0; bit < 4; ++bit) {
            result[bit] = value[(block * 4) + bit];
        }
        return result;
    }
};

using HierarchicalCLA16 = HierarchialCLA16;

#endif
