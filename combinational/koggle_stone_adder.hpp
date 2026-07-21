#ifndef KOGGE_STONE_ADDER_HPP
#define KOGGE_STONE_ADDER_HPP

#include <bitset>
#include <array>
#include <cstddef>

template<std::size_t N>
struct PrefixAdderResult
{
    std::bitset<N> sum;
    bool carry_out;
};

template<std::size_t N>
class KoggeStoneAdder
{
    static_assert(N > 0);

private:

    struct GP
    {
        bool g;
        bool p;
    };

    std::bitset<N> A;
    std::bitset<N> B;

public:

    KoggeStoneAdder(
        const std::bitset<N>& a,
        const std::bitset<N>& b)
        : A(a), B(b)
    {
    }

    PrefixAdderResult<N> Result(bool Cin = false) const
    {
        std::array<GP,N> stage;

        //----------------------------------
        // Initial Generate / Propagate
        //----------------------------------

        for(std::size_t i=0;i<N;++i)
        {
            stage[i].g = A[i] & B[i];
            stage[i].p = A[i] ^ B[i];
        }

        //----------------------------------
        // Kogge-Stone Prefix Tree
        //----------------------------------

        for(std::size_t distance=1;
            distance < N;
            distance <<= 1)
        {
            auto previous = stage;

            for(std::size_t i=distance;
                i<N;
                ++i)
            {
                stage[i].g =
                    previous[i].g
                    |
                    (
                        previous[i].p
                        &
                        previous[i-distance].g
                    );

                stage[i].p =
                    previous[i].p
                    &
                    previous[i-distance].p;
            }
        }

        //----------------------------------
        // Carries
        //----------------------------------

        std::array<bool,N+1> carry{};

        carry[0] = Cin;

        for(std::size_t i=0;i<N;++i)
        {
            carry[i+1] =
                stage[i].g
                |
                (
                    stage[i].p
                    &
                    Cin
                );
        }

        //----------------------------------
        // Sum
        //----------------------------------

        PrefixAdderResult<N> result;

        for(std::size_t i=0;i<N;++i)
        {
            bool p =
                A[i] ^ B[i];

            result.sum[i] =
                p ^ carry[i];
        }

        result.carry_out =
            carry[N];

        return result;
    }
};

#endif