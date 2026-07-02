#ifndef HALF_ADDER_HPP
#define HALF_ADDER_HPP

#include "Gates.h"
#include <bitset>

class HalfAdder
{
private:
    std::bitset<2> inputs;

public:
    HalfAdder(bool a, bool b)
    {
        inputs[0] = a;
        inputs[1] = b;
    }

    explicit HalfAdder(const std::bitset<2>& in)
        : inputs(in) {}

    bool sum() const
    {
        return Gates(inputs).XOR();
    }

    bool carry() const
    {
        return Gates(inputs).AND();
    }

    std::bitset<2> output() const
    {
        std::bitset<2> result;
        result[0] = sum();
        result[1] = carry();
        return result;
    }
};

#endif
