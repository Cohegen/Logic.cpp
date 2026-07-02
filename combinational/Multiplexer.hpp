/*
This program implements a 4:1 multiplexer
*/
#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include "Gates.h"
#include <bitset>
#include <cstddef>
#include <iostream>
#include <initializer_list>

class Multiplexer
{
private:
    std::bitset<4> inputs;
    std::bitset<2> select;
    bool valid{true};

public:
    explicit Multiplexer(std::initializer_list<bool> in)
    {
        if (in.size() != 4) {
            std::cerr << "Multiplexer expects 4 data inputs\n";
            valid = false;
            return;
        }

        std::size_t index = 0;
        for (bool bit : in) {
            inputs[index++] = bit;
        }
    }

    explicit Multiplexer(const std::bitset<4>& in)
        : inputs(in) {}

    void setSelectLines(bool a, bool b)
    {
        select[0] = a;
        select[1] = b;
    }

    void setSelectLines(const std::bitset<2>& lines)
    {
        select = lines;
    }

    bool output() const
    {
        if (!valid) {
            std::cerr << "Multiplexer expects 4 data inputs\n";
            return false;
        }

        const bool s0 = select[0];
        const bool s1 = select[1];
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
