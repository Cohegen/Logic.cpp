#ifndef HALF_ADDER_HPP
#define HALF_ADDER_HPP

#include "Gates.h"

class HalfAdder
{
private:
    bool a;
    bool b;

public:
    HalfAdder(bool input1, bool input2)
        : a(input1), b(input2) {}

    bool sum() const
    {
        const Gates gate(a, b);
        return gate.XOR();
    }

    bool carry() const
    {
        const Gates gate(a, b);
        return gate.AND();
    }
};

#endif
