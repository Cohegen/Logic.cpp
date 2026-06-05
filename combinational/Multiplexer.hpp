/*
This program implements a 4:1 multiplexer
*/
#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include "Gates.h"
#include <iostream>
#include <initializer_list>
#include <vector>

class Multiplexer
{
private:
    std::vector<bool> inputs;
    bool s0{false};
    bool s1{false};

public:
    Multiplexer(std::initializer_list<bool> in)
        : inputs(in)
    {
        if (inputs.size() != 4) {
            std::cerr << "Multiplexer expects 4 data inputs\n";
            inputs.clear();
        }
    }

    void setSelectLines(bool a, bool b)
    {
        s0 = a;
        s1 = b;
    }

    bool output() const
    {
        if (inputs.size() != 4) {
            std::cerr << "Multiplexer expects 4 data inputs\n";
            return false;
        }

        const bool notS0 = Gates{s0}.NOT();
        const bool notS1 = Gates{s1}.NOT();

        const bool term0 = Gates{inputs[0], notS1, notS0}.AND();
        const bool term1 = Gates{inputs[1], notS1, s0}.AND();
        const bool term2 = Gates{inputs[2], s1, notS0}.AND();
        const bool term3 = Gates{inputs[3], s1, s0}.AND();

        return Gates{term0, term1, term2, term3}.OR();
    }
};

#endif
