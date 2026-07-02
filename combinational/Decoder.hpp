/*
This program implements a 2:4 decoder
*/
#ifndef DECODER_HPP
#define DECODER_HPP

#include "Gates.h"
#include <bitset>
#include <cstddef>
#include <iostream>
#include <initializer_list>

class Decoder
{
private:
    std::bitset<2> inputs;
    bool valid{true};

public:
    explicit Decoder(std::initializer_list<bool> in)
    {
        if (in.size() != 2) {
            std::cerr << "Decoder expects 2 inputs\n";
            valid = false;
            return;
        }

        std::size_t index = 0;
        for (bool bit : in) {
            inputs[index++] = bit;
        }
    }

    explicit Decoder(const std::bitset<2>& in)
        : inputs(in) {}

    std::bitset<4> outputs() const
    {
        if (!valid) {
            std::cerr << "Decoder expects 2 inputs\n";
            return {};
        }

        const bool not0 = Gates{inputs[0]}.NOT();
        const bool not1 = Gates{inputs[1]}.NOT();

        std::bitset<4> result;
        result[0] = Gates{not0, not1}.AND();
        result[1] = Gates{not0, inputs[1]}.AND();
        result[2] = Gates{inputs[0], not1}.AND();
        result[3] = Gates{inputs[0], inputs[1]}.AND();
        return result;
    }
};

#endif
