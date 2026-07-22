/*
Here we define a Buffer.
It copies the input to output .

*/
#pragma  once
#include "Gates.hpp"
#include "Signals/wire.hpp"
#pragma once

namespace logic {
    class BufferGate:public Gate{
        public: 
           BufferGate(Wire& input,Wire& output):m_input(input),m_output(output){}

           void evaluate() override
           {
            m_output.write(m_input.read());
           }

        private:
        Wire& m_input;
        Wire& m_output;
    };

}
