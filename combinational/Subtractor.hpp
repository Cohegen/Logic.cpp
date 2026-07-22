#ifndef SUBTRACTOR_HPP
#define SUBTRACTOR_HPP

#include <bitset>

template <std::size_t N>
class Subtractor {
private:
    std::bitset<N> A;
    std::bitset<N> B;

public:
    Subtractor(std::bitset<N> a,
               std::bitset<N> b)
        : A(a), B(b)
    {}

    std::bitset<N> Result()
    {
        unsigned long result =
            A.to_ulong() - B.to_ulong();

        return std::bitset<N>(result);
    }
};

#endif