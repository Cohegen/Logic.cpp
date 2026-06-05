/*
This program implements a 2:4 decoder
*/
#ifndef DECODER_HPP
#define DECODER_HPP

#include "Gates.h"
#include <iostream>
#include <initializer_list>
#include <vector>

class Decoder
{
private:
    std::vector<bool> inputs;

public:
    Decoder(std::initializer_list<bool> in)
        : inputs(in)
    {
        if (inputs.size() != 2) {
            std::cerr << "Decoder expects 2 inputs\n";
            inputs.clear();
        }
    }

    std::vector<bool> outputs() const
    {
        if (inputs.size() != 2) {
            std::cerr << "Decoder expects 2 inputs\n";
            return {false, false, false, false};
        }

        const bool not0 = Gates{inputs[0]}.NOT();
        const bool not1 = Gates{inputs[1]}.NOT();

        return {
            Gates{not0, not1}.AND(),
            Gates{not0, inputs[1]}.AND(),
            Gates{inputs[0], not1}.AND(),
            Gates{inputs[0], inputs[1]}.AND()
        };
    }
};

#endif
