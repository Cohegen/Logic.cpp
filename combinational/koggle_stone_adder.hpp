#ifndef KOGGLE_STONE_ADDER_HPP
#define KOGGLE_STONE_ADDER_HPP

#include <bitset>
#include <cstddef>

template <std::size_t N>
class KoggleStoneAdder
{
private:
    std::bitset<N> a;
    std::bitset<N> b;
    bool carryIn{false};

public:
    KoggleStoneAdder(const std::bitset<N>& lhs, const std::bitset<N>& rhs, bool cin = false)
        : a(lhs), b(rhs), carryIn(cin) {}

    std::bitset<N> sum() const
    {
        const auto carries = carryBits();
        const auto p = a ^ b;
        std::bitset<N> result;

        for (std::size_t i = 0; i < N; ++i) {
            result[i] = p[i] ^ carries[i];
        }

        return result;
    }

    bool carryOut() const
    {
        return carryBits()[N];
    }

    std::bitset<N + 1> carryBits() const
    {
        std::bitset<N> p = a ^ b;
        std::bitset<N> g = a & b;

        for (std::size_t distance = 1; distance < N; distance <<= 1) {
            const auto previousP = p;
            const auto previousG = g;

            for (std::size_t i = distance; i < N; ++i) {
                g[i] = previousG[i] || (previousP[i] && previousG[i - distance]);
                p[i] = previousP[i] && previousP[i - distance];
            }
        }

        std::bitset<N + 1> carry;
        carry[0] = carryIn;

        for (std::size_t i = 0; i < N; ++i) {
            carry[i + 1] = g[i] || (p[i] && carryIn);
        }

        return carry;
    }
};

template <std::size_t N>
using KoggeStoneAdder = KoggleStoneAdder<N>;

#endif
