/*
This program implements the NOT Gate.
The NOT gate has one input A and one output Y
*/

#include "Signals/wire.hpp"
#include "gates/Gates.hpp"
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
        //referencing two wires as our data members
        Wire& m_input;
        Wire& m_output;
    };

}
