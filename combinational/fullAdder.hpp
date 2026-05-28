#ifndef FULL_ADDER_HPP
#define FULL_ADDER_HPP

#include "Gates.h"

class FullAdder
{
private:
    bool A;
    bool B;
    bool Cin;

public:
    FullAdder(bool input1, bool input2, bool input3)
        : A(input1), B(input2), Cin(input3) {}

    bool Sum() const
    {
        const Gates firstXor(A, B);
        const Gates secondXor(firstXor.XOR(), Cin);
        return secondXor.XOR();
    }

    bool Carry() const
    {
        const Gates ab(A, B);
        const Gates bc(B, Cin);
        const Gates ac(A, Cin);

        const Gates firstOr(ab.AND(), bc.AND());
        const Gates secondOr(firstOr.OR(), ac.AND());

        return secondOr.OR();
    }
};

#endif
