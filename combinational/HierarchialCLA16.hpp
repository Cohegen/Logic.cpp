#ifndef HIERARCHICAL_CLA16_HPP
#define HIERARCHICAL_CLA16_HPP

#include "CLA4.hpp"
#include <bitset>

struct CLA16Result
{
    std::bitset<16> sum;
    bool carry_out;
};

class CLA16
{
private:
    std::bitset<16> A;
    std::bitset<16> B;

public:
    CLA16(const std::bitset<16>& a,
          const std::bitset<16>& b)
        : A(a), B(b)
    {}

    CLA16Result Result(bool Cin = false) const
    {
        CLA4 blocks[4] =
        {
            CLA4(
                std::bitset<4>((A.to_ulong() >> 0) & 0xF),
                std::bitset<4>((B.to_ulong() >> 0) & 0xF)
            ),

            CLA4(
                std::bitset<4>((A.to_ulong() >> 4) & 0xF),
                std::bitset<4>((B.to_ulong() >> 4) & 0xF)
            ),

            CLA4(
                std::bitset<4>((A.to_ulong() >> 8) & 0xF),
                std::bitset<4>((B.to_ulong() >> 8) & 0xF)
            ),

            CLA4(
                std::bitset<4>((A.to_ulong() >> 12) & 0xF),
                std::bitset<4>((B.to_ulong() >> 12) & 0xF)
            )
        };

        bool GG[4];
        bool GP[4];

        for(int i = 0; i < 4; ++i)
        {
            auto signals =
                blocks[i].GroupSignals();

            GG[i] =
                signals.group_generate;

            GP[i] =
                signals.group_propagate;
        }

        bool C0 = Cin;

        bool C4 =
            GG[0]
            |
            (GP[0] & C0);

        bool C8 =
            GG[1]
            |
            (GP[1] & GG[0])
            |
            (GP[1] & GP[0] & C0);

        bool C12 =
            GG[2]
            |
            (GP[2] & GG[1])
            |
            (GP[2] & GP[1] & GG[0])
            |
            (GP[2] & GP[1] & GP[0] & C0);

        bool C16 =
            GG[3]
            |
            (GP[3] & GG[2])
            |
            (GP[3] & GP[2] & GG[1])
            |
            (GP[3] & GP[2] & GP[1] & GG[0])
            |
            (GP[3] & GP[2] & GP[1] & GP[0] & C0);

        bool carries[4] =
        {
            C0,
            C4,
            C8,
            C12
        };

        CLA16Result result;

        for(int block = 0; block < 4; ++block)
        {
            auto partial =
                blocks[block].Result(
                    carries[block]
                );

            for(int bit = 0; bit < 4; ++bit)
            {
                result.sum[block * 4 + bit]
                    =
                    partial.sum[bit];
            }
        }

        result.carry_out = C16;

        return result;
    }
};

#endif