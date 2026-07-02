#ifndef FULL_ADDER_HPP
#define FULL_ADDER_HPP

#include "Gates.h"
#include <bitset>
#include <cstddef>
#include <iostream>
#include <initializer_list>

class FullAdder
{
private:
    std::bitset<3> inputs;
    bool valid{true};

public:
    FullAdder(std::initializer_list<bool> in)
    {
        if (in.size() != 3) {
            std::cerr << "FullAdder expects 3 inputs\n";
            valid = false;
            return;
        }

        std::size_t index = 0;
        for (bool bit : in) {
            inputs[index++] = bit;
        }
    }

    explicit FullAdder(const std::bitset<3>& in)
        : inputs(in) {}

    bool Sum() const
    {
        if (!valid) {
            std::cerr << "FullAdder expects 3 inputs\n";
            return false;
        }
        return Gates(inputs).XOR();
    }

    bool Carry() const
    {
        if (!valid) {
            std::cerr << "Full Adder expects 3 inputs\n";
            return false;
        }

        const bool A = inputs[0];
        const bool B = inputs[1];
        const bool Cin = inputs[2];

        const Gates ab(A, B);
        const Gates bc(B, Cin);
        const Gates ac(A, Cin);

        return Gates{ab.AND(), bc.AND(), ac.AND()}.OR();
    }

    std::bitset<2> output() const
    {
        std::bitset<2> result;
        result[0] = Sum();
        result[1] = Carry();
        return result;
    }
};

#endif
