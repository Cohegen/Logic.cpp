/*
 * RingCounter.hpp
 * 
 * An implementation of an N-bit Ring Counter using modular gate-level logic components,
 * 2-to-1 multiplexers, and a sequential D-flip-flop register.
 *
 * =========================================================================================
 * BEHAVIOR & FUNCTIONALITY:
 * =========================================================================================
 * A Ring Counter is a circular shift register counter. In an N-bit ring counter:
 * 1. State Encoding: The counter state is represented using a one-hot encoding where exactly
 *    one bit is HIGH (1) and all other (N - 1) bits are LOW (0).
 * 2. Initial / Reset State:
 *    - Reset value: 0b00...0001 (bit 0 = HIGH, bits 1..N-1 = LOW).
 *    - When `reset` wire is HIGH (1), the counter synchronous reset sets the next register
 *      input to `initial_state_` on the active clock edge.
 * 3. Counting / Rotation (`enable` HIGH, `reset` LOW):
 *    - On each clock cycle, the single active HIGH bit rotates right circularly:
 *        count[0]   <- count[N-1]
 *        count[1]   <- count[0]
 *        count[2]   <- count[1]
 *        ...
 *        count[N-1] <- count[N-2]
 *    - The counter sequences through N distinct one-hot states (e.g., for 4-bit: 0001 -> 0010 -> 0100 -> 1000 -> 0001).
 * 4. Hold State (`enable` LOW, `reset` LOW):
 *    - The counter holds its current state regardless of clock transitions.
 *
 * =========================================================================================
 * DATAPATH ARCHITECTURE & SIGNAL FLOW:
 * =========================================================================================
 *
 *                  +-------------------------------------------------------------+
 *                  |                                                             |
 *                  v                                                             |
 *             +---------+     +--------------+     +-------------+     +------+   |
 *             |  Bus    |---->| enable_muxes |---->| reset_muxes |---->| reg_ |---+
 *             | shift   |     |  (Mux2to1)   |     |  (Mux2to1)  |     |(D-FF)|---> Output count_
 *             +---------+     +--------------+     +-------------+     +------+
 *                                    ^                    ^               ^
 *                                    |                    |               |
 *                                 enable_               reset_       clock_wire_
 *                                                         ^
 *                                                         |
 *                                                  initial_state_
 *                                                  (00...0001)
 *
 * Datapath stages per bit `i` (0 <= i < N):
 * 1. Circular Shift Wiring (`shifted_count_`):
 *    - `shifted_count_[0] = count_[N-1]`
 *    - `shifted_count_[i] = count_[i-1]` for `i > 0`
 * 2. Enable Stage (`enable_muxes_[i]`):
 *    - Inputs: 0 -> `count_[i]` (Hold), 1 -> `shifted_count_[i]` (Shift)
 *    - Select: `enable_`
 *    - Output: `count_next_[i]`
 * 3. Reset Stage (`reset_muxes_[i]`):
 *    - Inputs: 0 -> `count_next_[i]` (Normal), 1 -> `initial_state_[i]` (Reset)
 *    - Select: `reset_`
 *    - Output: `reg_input_[i]`
 * 4. Storage Stage (`reg_`):
 *    - Input: `reg_input_`
 *    - Clock: `clock_wire_`
 *    - Output: `count_`
 */

#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include "simulator/Component.hpp"
#include "combinational/multiplexers/Mux2to1.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"

namespace logic {

    template<std::size_t N>
    class RingCounter : public Component {
        static_assert(N > 0, "RingCounter width must be greater than zero.");

    public:
        RingCounter(
            Clock& clock,
            Wire& enable,
            Wire& reset,
            Bus<N>& count
        ) : clock_(clock),
            enable_(enable),
            reset_(reset),
            count_(count),
            clock_wire_(clock.state()),
            count_next_(),
            shifted_count_(),
            initial_state_(),
            reg_input_(),
            enable_muxes_(make_enable_muxes(std::make_index_sequence<N>{})),
            reset_muxes_(make_reset_muxes(std::make_index_sequence<N>{})),
            reg_(reg_input_, clock_wire_, count_)
        {
            // Initializing initial state constant to one-hot value 000...0001
            initial_state_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                initial_state_[i].write(LogicState::LOW);
            }
        }

        void evaluate() noexcept override
        {
            // Sync internal clock wire state
            clock_wire_.write(clock_.state());

            // Ensure constant one-hot initial_state_ signal values (000...0001)
            initial_state_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                initial_state_[i].write(LogicState::LOW);
            }

            // Datapath Step 1: Compute circular right-shifted count bus
            shifted_count_[0].write(count_[N - 1].read());
            for (std::size_t i = 1; i < N; ++i) {
                shifted_count_[i].write(count_[i - 1].read());
            }

            // Datapath Step 2 & 3: Evaluate multiplexer arrays (Enable and Reset)
            for (std::size_t i = 0; i < N; ++i) {
                enable_muxes_[i].evaluate();
                reset_muxes_[i].evaluate();
            }

            // Datapath Step 4: Clock update on storage register
            reg_.evaluate();
        }

    private:
        // Input / Output External References
        Clock& clock_;
        Wire& enable_;
        Wire& reset_;
        Bus<N>& count_;

        // Internal Signals
        Wire clock_wire_;
        Bus<N> count_next_;
        Bus<N> shifted_count_;
        Bus<N> initial_state_;
        Bus<N> reg_input_;

        // Internal Hardware Components
        std::array<Mux2to1, N> enable_muxes_;
        std::array<Mux2to1, N> reset_muxes_;
        Register<N> reg_;

        // Helper template to construct enable multiplexers
        template<std::size_t... I>
        std::array<Mux2to1, N> make_enable_muxes(std::index_sequence<I...>)
        {
            return {Mux2to1(count_[I], shifted_count_[I], enable_, count_next_[I])...};
        }

        // Helper template to construct reset multiplexers
        template<std::size_t... I>
        std::array<Mux2to1, N> make_reset_muxes(std::index_sequence<I...>)
        {
            return {Mux2to1(count_next_[I], initial_state_[I], reset_, reg_input_[I])...};
        }
    };

} 
