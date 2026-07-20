#pragma once 

#include <vector>
#include "wire.hpp"

namespace dd {
    class Bus
    {
        public:
            explicit Bus(std::size_t width): m_wite(width){}

            //Returns the size of the bus in terms of number of wires
            std::size_t width() const 
            {
                return m_wires.size();
            }

            Wire& operator[](std::size_t i)
            {
                return m_wires[i];
            }

            const Wire& operator[](std::size_t i) const 
            {
                return m_wires[i];
            }

        private:

        std::vector<Wire> m_wire;
    }
};
