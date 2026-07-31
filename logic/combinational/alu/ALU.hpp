/*
Implementation of the ALU
*/
#pragma once
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "combinational/adders/RippleCarryAdder.hpp"
#include "combinational/subtractors/FullSubtractor.hpp"
#include "gates/AND.hpp"
#include "gates/OR.hpp"
#include "gates/XOR.hpp"
#include "combinational/multiplexers/Mux4to1.hpp"

namespace logic{
    template<std::size_t N>
    class ALU{

        public:
          ALU(
            Bus& a,
            Bus& b,
            Bus& op,
          ): A(a),
             B(b),
             opcode(op)
             {}
           void evaluate(){
               const Bus val = opcode.read();


           }


        private:
        Bus<N> A;
        Bus<N> B;
        Bus<3>opcode;

        //outputs
        Bus<N> Result;

        Wire ZeroFlag;
        Wire CarryFlag;
        Wire OverFlowFlag;


    }
}