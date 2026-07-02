#ifndef GATES_H
#define GATES_H

#include <bitset>
#include <cstddef>
#include <initializer_list>

class Gates
{
private:
    static constexpr std::size_t maxInputs = 64;
    std::bitset<maxInputs> inputs;
    std::size_t inputCount{0};

public:
    Gates(bool input1, bool input2);
    Gates(bool input1);
    Gates(std::initializer_list<bool> in);

    template <std::size_t N>
    explicit Gates(const std::bitset<N>& in) : inputCount(N)
    {
        static_assert(N <= maxInputs, "Gates supports up to 64 inputs");
        for (std::size_t i = 0; i < N; ++i) {
            inputs[i] = in[i];
        }
    }

    bool AND() const;
    bool OR() const;
    bool NOT() const;
    bool BUFFER() const;
    bool XOR() const;
    bool NAND() const;
    bool NOR() const;
    bool XNOR() const;
};

#endif
