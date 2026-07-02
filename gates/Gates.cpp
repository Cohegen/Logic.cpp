/*
This program puts all logic gates in a common class
for easy use in future modules
*/
#include "Gates.h"

#include <iostream>

Gates::Gates(bool input1, bool input2)
    : inputCount(2)
{
    inputs[0] = input1;
    inputs[1] = input2;
}

Gates::Gates(bool input1)
    : inputCount(1)
{
    inputs[0] = input1;
}

Gates::Gates(std::initializer_list<bool> in)
    : inputCount(in.size())
{
    if (inputCount > maxInputs) {
        std::cerr << "Gate supports up to 64 inputs\n";
        inputCount = 0;
        return;
    }

    std::size_t index = 0;
    for (bool bit : in) {
        inputs[index++] = bit;
    }
}

bool Gates::AND() const {
    for (std::size_t i = 0; i < inputCount; ++i) {
        if (!inputs[i]) {
            return false;
        }
    }
    return true;
}

bool Gates::OR() const {
    for (std::size_t i = 0; i < inputCount; ++i) {
        if (inputs[i]) {
            return true;
        }
    }
    return false;
}

bool Gates::NOT() const {
    if (inputCount != 1) {
        std::cerr << "NOT gate requires exactly one input\n";
        return false;
    }
    return !inputs[0];
}

bool Gates::BUFFER() const {
    if (inputCount != 1) {
        std::cerr << "Buffer gate requires exactly one input\n";
        return false;
    }
    return inputs[0];
}

bool Gates::XOR() const {
    bool result = false;
    for (std::size_t i = 0; i < inputCount; ++i) {
        result = result ^ inputs[i];
    }
    return result;
}

bool Gates::NAND() const {
    return !AND();
}

bool Gates::NOR() const {
    return !OR();
}

bool Gates::XNOR() const {
    return !XOR();
}
