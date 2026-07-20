#pragma once 
#include "signal.hpp"

namespace dd {
    class Wire
    {
        public:

        /*
        A method whose role is to change the value of the signal
        */
        void write(bool value)
        {
            m_signal.set(value);
        }

        /*
        Reading the signal value in the wire
        */
        bool read() const{
            return m_signal.value();
        }

     private:

     Signal m_signal;

    };
}
