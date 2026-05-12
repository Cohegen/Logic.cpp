/*
This program puts all logic gates in a common class
for easy use in future modules
*/
#include "Gates.h"

// Constructors
Gates::Gates(bool input1, bool input2)
    : a(input1), b(input2) {}

Gates::Gates(bool input1)
    : a(input1), b(false) {}

// Gates
bool Gates::AND() const { return a && b; }

bool Gates::OR() const { return a || b; }

bool Gates::NOT() const { return !a; }

bool Gates::BUFFER() const { return a; }

bool Gates::XOR() const { return a != b; }

bool Gates::NAND() const { return !(a && b); }

bool Gates::NOR() const { return !(a || b); }

bool Gates::XNOR() const { return a == b; }
