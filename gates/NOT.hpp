/*
This program implements the NOT Gate.
The NOT gate has one input A and one output Y
*/

#pragma once
#include "Gates.hpp"
#include "Signals/wire.hpp"

namespace logic {
    class NotGate:public Gate{
        public:
           NotGate(Wire& input,Wire& output):m_input(input),m_output(output){}

           void evaluate() override
           {
            m_output.write(
                logic_not(m_input.read())
            );
           }
        private:
        // Input and output wires connected to this gate.
        Wire& m_input;
        Wire& m_output;
    };

}