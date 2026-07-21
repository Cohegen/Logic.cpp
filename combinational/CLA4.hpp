/*
An implementation of a 4-bit CLA block
*/

#ifndef CLA4_HPP
#define CLA4_HPP

#include <bitset>

struct CLA4Result
{
    std::bitset<4> sum;
    bool carry_out;
};

struct CLA4GroupSignals
{
    bool group_generate;
    bool group_propagate;
};

class CLA4
{
private:
    std::bitset<4> A;
    std::bitset<4> B;

public:
    CLA4(const std::bitset<4>& a,
         const std::bitset<4>& b)
        : A(a), B(b)
    {}

    CLA4GroupSignals GroupSignals() const
    {
        bool G[4];
        bool P[4];

        for(int i = 0; i < 4; ++i)
        {
            G[i] = A[i] & B[i];
            P[i] = A[i] ^ B[i];
        }

        bool GP =
            P[3] &
            P[2] &
            P[1] &
            P[0];

        bool GG =
            G[3]
            |
            (P[3] & G[2])
            |
            (P[3] & P[2] & G[1])
            |
            (P[3] & P[2] & P[1] & G[0]);

        return {GG, GP};
    }

    CLA4Result Result(bool Cin) const
    {
        bool G[4];
        bool P[4];

        for(int i = 0; i < 4; ++i)
        {
            G[i] = A[i] & B[i];
            P[i] = A[i] ^ B[i];
        }

        bool C0 = Cin;

        bool C1 =
            G[0] |
            (P[0] & C0);

        bool C2 =
            G[1] |
            (P[1] & G[0]) |
            (P[1] & P[0] & C0);

        bool C3 =
            G[2] |
            (P[2] & G[1]) |
            (P[2] & P[1] & G[0]) |
            (P[2] & P[1] & P[0] & C0);

        bool C4 =
            G[3] |
            (P[3] & G[2]) |
            (P[3] & P[2] & G[1]) |
            (P[3] & P[2] & P[1] & G[0]) |
            (P[3] & P[2] & P[1] & P[0] & C0);

        CLA4Result result;

        result.sum[0] = P[0] ^ C0;
        result.sum[1] = P[1] ^ C1;
        result.sum[2] = P[2] ^ C2;
        result.sum[3] = P[3] ^ C3;

        result.carry_out = C4;

        return result;
    }
};

#endif