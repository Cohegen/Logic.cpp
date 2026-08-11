#pragma once
#include <array>
#include <utility>
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "combinational/decoders/Dec2to4.hpp"
#include "combinational/multiplexers/Mux4to1.hpp"
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"
#include "simulator/Component.hpp"
#include "simulator/Circuit.hpp"
#include "gates/AND.hpp"
namespace logic{
    template<std::size_t AddressWidth, std::size_t DataWidth>
    class RAM:public Component{
        public: 
         RAM(
            Clock& clock,
            Wire& reset,
            Wire& read_enable,
            Wire& write_enable,
            Bus<N>& address,
            Bus<N>& write_data,
            Bus<N>& read_data
         ){}

        private:
           //inputs
           Clock& clock_;
           Wire& reset_;
           Wire& read_enable_;
           Wire& write_enable_;
           Bus<N>& address_;
           //operations
           Bus<N>& write_data_;
           Bus<N>& read_data_;

           Wire clock_wire_;

           //internal components
           Dec2to4 decoder_;
           std::array<Register<N>,4> registers_;
           std::array<Mux2to1, N> read_enable_muxes_;

           //Decoder outputs
           std::array<Wire,4>word_select_;

           //write enables
           std::array<Wire,4>word_write_enable_;

           //memory words
           std::array<Bus<N>,4>memory_;

           // Read path
           Bus<N> selected_data_;
           Bus<N> zero_bus_;

           

    };

}