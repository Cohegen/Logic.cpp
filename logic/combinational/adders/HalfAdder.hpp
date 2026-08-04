/*
implementation of a half adder
*/

#pragma once
#include "gates/Gate.hpp"
#include "signals/wire.hpp"
#include "gates/XOR.hpp"
#include "gates/AND.hpp"
#include "simulator/Component.hpp"

namespace logic
{
    class HalfAdder:public Component
    {
         public:
           HalfAdder(Wire& a,Wire& b,Wire& sum,Wire& carry):
                m_sumGate(a,b,sum),m_carryGate(a,b,carry){}

            void evaluate()  
            {
                m_sumGate.evaluate();
                m_carryGate.evaluate();
            }


        private:
           XorGate m_sumGate;
           ANDGate m_carryGate;

            

            
    };
}
