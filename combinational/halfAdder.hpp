#ifndef HALF_ADDER_HPP
#define HALF_ADDER_HPP

#include "Gates.h"

class HalfAdder
{
private:
    Gates gates;

public:
    HalfAdder(bool a, bool b)
        : gates{a, b} {}

    bool sum() const
    {
        return gates.XOR();
    }

    bool carry() const
    {
        return gates.AND();
    }
};

#endif
