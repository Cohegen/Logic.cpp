#ifndef FULL_ADDER_HPP
#define FULL_ADDER_HPP

#include "Gates.h"
#include <iostream>
#include <vector>

class FullAdder
{
private:
   //Full adder takes in three inputs
   std::vector<bool>inputs;

public:
    FullAdder(std::initializer_list<bool>in)
        : inputs(in) {}

    bool Sum() const
    {
        if(inputs.size()!=3)
        {
            std::cerr<< "FullAdder expects 3 inputs\n";
            return false;
        }
        const Gates SumXor(inputs);
        return SumXor.XOR();
    }

    bool Carry() const
    {
        if(inputs.size()!=3){
            std::cerr << "Full Adder expects 3 inputs\n";
            return false;
        }
        
        const bool A = inputs[0];
        const bool B = inputs[1];
        const bool Cin = inputs[2];

       const Gates ab(A,B);
       const Gates bc(B,Cin);
       const Gates ac(A,Cin);

       const  Gates result(ab.AND(),bc.AND(),ac.AND());
       return result.OR();

       
    }
};

#endif
