/*
An implementation of a Johnson counter
*/
#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include "simulator/Component.hpp"
#include "gates/NOT.hpp"
#include "combinational/multiplexers/Mux.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"

/**
 * @brief N-bit Synchronous Johnson Counter (Twisted Ring Counter)
 * 
 * ### Behavioral Description:
 * A Johnson counter (also known as a switch-tail or twisted ring counter) is a modified ring counter 
 * where the inverted output of the most significant flip-flop (MSB, `count[N-1]`) is fed back into 
 * the input of the least significant flip-flop (`shifted_count[0]`).
 * 
 * - **State Sequence & Length**: An N-bit Johnson counter cycles through 2*N unique states before repeating.
 *   - For N=4: 0000 -> 0001 -> 0011 -> 0111 -> 1111 -> 1110 -> 1100 -> 1000 -> 0000.
 * - **Control Signals**:
 *   - `reset` (HIGH): Synchronously clears all output bits in `count` to `00...0` (LogicState::LOW).
 *   - `enable` (HIGH): Enables shift and count evaluation on clock transitions. When LOW, holds current count value.
 * 
 * ### Datapath & Circuit Structure:
 * 1. **Feedback Gate (`feedback_not_`)**:
 *    Inverts the MSB register output: `feedback = NOT(count[N-1])`.
 * 2. **Shift Logic (`shifted_count_`)**:
 *    - Bit 0: Driven by inverted feedback wire (`shifted_count[0] = feedback`).
 *    - Bits 1 to N-1: Driven by adjacent lower register bits (`shifted_count[i] = count[i-1]`).
 * 3. **Enable Multiplexers (`enable_muxes_`)**:
 *    - `S = enable`
 *    - `In0 = count[i]` (Hold state when enable = 0)
 *    - `In1 = shifted_count[i]` (Next shifted state when enable = 1)
 *    - `Out = count_next[i]`
 * 4. **Synchronous Reset Multiplexers (`reset_mux_`)**:
 *    - `S = reset`
 *    - `In0 = count_next[i]` (Normal count/hold state when reset = 0)
 *    - `In1 = zero_bus_` (Force LOW when reset = 1)
 *    - `Out = reg_input[i]`
 * 5. **N-bit Register (`reg_`)**:
 *    Latches `reg_input` to `count` on active clock edge evaluation.
 */
namespace logic{
    template<std::size_t N>
    class JohnsonCounter:public Component{
        static_assert(
            N > 0,
            "JohnsonCounter width must be greater than zero."
        );

        public:
        JohnsonCounter(
            Clock& clock,
            Wire& enable,
            Wire& reset,
            Bus<N>& count
        ) : clock_(clock),
            enable_(enable),
            reset_(reset),
            count_(count),
            clock_wire_(clock.state()),
            feedback_(LogicState::LOW),
            shifted_count_(),
            count_next_(),
            reg_input_(),
            feedback_not_(count_[N - 1], feedback_),
            enable_mux_(count_, shifted_count_, enable_, count_next_),
            reset_mux_(count_next_, zero_bus_, reset_, reg_input_),
            reg_(reg_input_, clock_wire_, count_)
        {
            feedback_.write(LogicState::LOW);
        }

        void evaluate() noexcept override
        {
             // Sync internal clock wire state
             clock_wire_.write(clock_.state());

             // Evaluate inverted feedback from MSB count[N-1]
             feedback_not_.evaluate();

             // Shift datapath connections
             shifted_count_[0].write(feedback_.read());

             for(std::size_t i=1;i<N;++i)
             {
                shifted_count_[i].write(count_[i-1].read());
             }

             // Select next state based on enable & reset control signals
             enable_mux_.evaluate();
             reset_mux_.evaluate();

            // Update state register
            reg_.evaluate();
        }

        private:
        Clock& clock_;
        Wire& enable_;
        Wire& reset_;
        Bus<N>& count_;

        // Internal wires and signals
        Wire clock_wire_;
        Wire feedback_{LogicState::LOW};

        Bus<N> zero_bus_;
        Bus<N> shifted_count_;
        Bus<N> count_next_;
        Bus<N> reg_input_;

        // Internal hardware components
        NotGate feedback_not_;
        Mux<N> enable_mux_;
        Mux<N> reset_mux_;
        Register<N> reg_;
    };
        
}
