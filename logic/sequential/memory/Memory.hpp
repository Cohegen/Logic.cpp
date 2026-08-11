/*
An implementation of memory interface
*/

#pragma once
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "combinational/decoders/Dec2to4.hpp"
#include ""
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"
#include "simulator/Component.hpp"
#include "simulator/Circuit.hpp"
namespace logic{
    template<std::size_t AddressWidth, std::size_t DataWidth>
    class Memory:public Component{

    };

}
