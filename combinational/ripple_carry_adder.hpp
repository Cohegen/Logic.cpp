/*
An implementation of a Ripple Carry Adder
*/
#ifndef RIPPLE_CARRY_ADDER_HPP
#define RIPPLE_CARRY_ADDER_HPP

#include <bitset>

template <std::size_t N>
struct AdderResult
{
    std::bitset<N> sum;
    bool carry_out;
};

template <std::size_t N>
class RCA
{
private:
    std::bitset<N> A;
    std::bitset<N> B;

public:
    RCA(const std::bitset<N>& a, const std::bitset<N>& b)
        : A(a), B(b)
    {
    }

    AdderResult<N> Result() const
    {
        AdderResult<N> result;

        bool carry = false; // Initial carry-in

        for (std::size_t i = 0; i < N; ++i)
        {
            // Sum bit
            result.sum[i] = A[i] ^ B[i] ^ carry;

            // Carry-out of current full adder
            carry = (A[i] & B[i]) |
                    (carry & (A[i] ^ B[i]));
        }

        result.carry_out = carry;

        return result;
    }
};

#endif