#ifndef HALF_ADDER_HPP
#define HALF_ADDER_HPP

#include "Gates.h"
#include <vector>

class HalfAdder
{
private:
  std::vector<bool>inputs;

public:
    HalfAdder(std::intializer_list<bool>in):inputs(std::move(in)){}

    bool sum() const
    {
        const Gates gate(inputs);
        return gate.XOR();
    }

    bool carry() const
    {
        const Gates gate(inputs);
        return gate.AND();
    }
};

#endif
