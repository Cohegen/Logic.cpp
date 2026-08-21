/*
Here we define a Buffer.
It copies the input to output .

*/
#pragma once
#include "gates/Gate.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic {
    class BufferGate:public Component{
        public: 
           BufferGate(Wire& input,Wire& output):m_input(input),m_output(output){}

           void evaluate() noexcept override
           {
            m_output.write(m_input.read());
           }

        private:
        Wire& m_input;
        Wire& m_output;
    };

}
