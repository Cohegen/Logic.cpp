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
 #include "combinational/multiplexers/Mux.hpp"
 #include "signals/bus.hpp"
 #include "signals/wire.hpp"
 #include "sequential/registers/register.hpp"
 #include "signals/clock.hpp"
 
 namespace logic{
     /*
      * Datapath:
      *   Register -> RippleBorrowSubtractor -> Enable MUX -> Reset MUX -> Register
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
               enable_mux_(count_, decrement_next_, enable_, count_next_),
               reset_mux_(count_next_, zero_bus_, reset_, reg_input_),
               reg_(reg_input_,clock_wire_,count_)
            {
               constant_one_[0].write(LogicState::HIGH);
               for(std::size_t i=1;i<N;++i){
                 constant_one_[i].write(LogicState::LOW);
               }
               borrow_in_.write(LogicState::LOW);
            }
 
            void evaluate() noexcept override
            {
              clock_wire_.write(clock_.state());
 
              // Compute count - 1 using the ripple borrow subtractor.
              borrow_subtractor_.evaluate();
 
              enable_mux_.evaluate();
              reset_mux_.evaluate();
 
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
 
         Bus<N> constant_one_;
         Bus<N> zero_bus_;
         Bus<N> decrement_next_;
         Bus<N> count_next_;
         Bus<N> reg_input_;
 
         RippleBorrowSubtractor<N> borrow_subtractor_;
         Mux<N> enable_mux_;
         Mux<N> reset_mux_;
         Register<N> reg_;
     };
 }