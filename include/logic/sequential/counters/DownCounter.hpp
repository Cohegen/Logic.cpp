/*
 * A synchronous N-bit binary down counter.
 *
 * Behavior:
 *  - Reset asserted  -> counter loads zero.
 *  - Enable asserted -> counter decrements by one.
 *  - Otherwise       -> counter retains its current value.
 */

 #pragma once
 #include <array>
 #include <cstddef>
 #include <utility>
 
 #include "simulator/Component.hpp"
 #include "combinational/subtractors/RippleBorrowSubtractor.hpp"
 #include "combinational/multiplexers/Mux2to1.hpp"
 #include "signals/bus.hpp"
 #include "signals/wire.hpp"
 #include "sequential/registers/register.hpp"
 #include "signals/clock.hpp"
 
 namespace logic{
     /*
      * Datapath:
      *   Register -> RippleBorrowSubtractor -> Enable MUXes -> Reset MUXes -> Register
      */
     template<std::size_t N>
     class DownCounter:public Component{
         static_assert(N>0,"DownCounter width must be greater than 0");
 
         public:
            DownCounter(
             Clock& clock,
             Wire& enable,
             Wire& reset,
             Bus<N>&count
            ): clock_(clock),
               enable_(enable),
               reset_(reset),
               count_(count),
               clock_wire_(clock.state()),
               borrow_subtractor_(count_,constant_one_,borrow_in_,decrement_next_,borrow_out_),
               enable_muxes_(make_enable_muxes(std::make_index_sequence<N>{})),
               reset_muxes_(make_reset_muxes(std::make_index_sequence<N>{})),
               reg_(reg_input_,clock_wire_,count_)
            {
               constant_one_[0].write(LogicState::HIGH);
               for(std::size_t i=1;i<N;++i){
                 constant_one_[i].write(LogicState::LOW);
               }
               borrow_in_.write(LogicState::LOW);
               zero_wire_.write(LogicState::LOW);
            }
 
            void evaluate() noexcept override
            {
              clock_wire_.write(clock_.state());
 
              // Compute count - 1 using the ripple borrow subtractor.
              borrow_subtractor_.evaluate();
 
              for(std::size_t i=0;i<N;++i){
                 enable_muxes_[i].evaluate();
                 reset_muxes_[i].evaluate();
              }
 
              reg_.evaluate();
            }
 
 
 
         private:
         Clock& clock_;
         Wire& enable_;
         Wire& reset_;
         Bus<N>&count_;
 
         Wire clock_wire_;
         Wire borrow_in_{LogicState::LOW};
         Wire borrow_out_{LogicState::LOW};
         Wire zero_wire_{LogicState::LOW};
 
         Bus<N> constant_one_;
         Bus<N> decrement_next_;
         Bus<N> count_next_;
         Bus<N> reg_input_;
 
         RippleBorrowSubtractor<N> borrow_subtractor_;
         std::array<Mux2to1, N> enable_muxes_;
         std::array<Mux2to1, N> reset_muxes_;
         Register<N> reg_;
 
         template<std::size_t... I>
         std::array<Mux2to1, N> make_enable_muxes(std::index_sequence<I...>)
         {
             return {Mux2to1(count_[I], decrement_next_[I], enable_, count_next_[I])...};
         }
 
         template<std::size_t... I>
         std::array<Mux2to1, N> make_reset_muxes(std::index_sequence<I...>)
         {
             return {Mux2to1(count_next_[I], zero_wire_, reset_, reg_input_[I])...};
         }
     };
 }
 