/*
This program puts all logic gates in a common class
for easy use in future modules
*/
#include "Gates.h"

#include <iostream>
#include <utility>

Gates::Gates(bool input1, bool input2)
    : inputs{input1, input2} {}

Gates::Gates(bool input1)
    : inputs{input1} {}

Gates::Gates(std::initializer_list<bool> in)
    : inputs(in) {}

Gates::Gates(std::vector<bool> in)
    : inputs(std::move(in)) {}

bool Gates::AND() const {
    for (bool bit : inputs) {
        if (!bit) {
            return false;
        }
    }
    return true;
}

bool Gates::OR() const {
    for (bool bit : inputs) {
        if (bit) {
            return true;
        }
    }
    return false;
}

bool Gates::NOT() const {
    if (inputs.size() != 1) {
        std::cerr << "NOT gate requires exactly one input\n";
        return false;
    }
    return !inputs[0];
}

bool Gates::BUFFER() const {
    if (inputs.size() != 1) {
        std::cerr << "Buffer gate requires exactly one input\n";
        return false;
    }
    return inputs[0];
}

bool Gates::XOR() const {
    bool result = false;
    for (bool bit : inputs) {
        result = result ^ bit;
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
