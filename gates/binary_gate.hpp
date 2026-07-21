/*
An intermediate base class
*/
#pragma once
#include "Gates.hpp"
#include "Signals/wire.hpp"

namespace logic {
    class BinaryGate: public Gate
{
    protected:
        BinaryGate(Wire& a,Wire& b,Wire& out):
               m_inputA(a),m_inputB(b),m_output(out){}

        Wire& m_inputA;
        Wire& m_inputB;
        Wire& m_output;
};

}

