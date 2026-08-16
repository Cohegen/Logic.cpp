/*
 * A synchronous N-bit binary up/down counter.
 *
 * Behavior:
 *  - Reset asserted     -> counter loads zero.
 *  - Direction HIGH (1) -> counter increments by one (UP).
 *  - Direction LOW (0)  -> counter decrements by one (DOWN).
 *  - Enable LOW (0)     -> counter retains its current value.
 *
 * Datapath:
 *   Register (count) -> RippleCarryAdder (count + 1) / RippleBorrowSubtractor (count - 1)
 *                    -> Direction MUXes (select incremented vs decremented value)
 *                    -> Enable MUXes (select updated value vs current count)
 *                    -> Reset MUXes (select zero vs enabled value)
 *                    -> Register
 */
#pragma once
#include <array>
#include <utility>
#include <cstddef>
#include "simulator/Component.hpp"
#include "combinational/subtractors/RippleBorrowSubtractor.hpp"
#include "combinational/adders/RippleCarryAdder.hpp"
#include "combinational/multiplexers/Mux.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"

namespace logic {
    template<std::size_t N>
    class UpDownCounter : public Component {
        static_assert(N > 0, "UpDownCounter width must be greater than 0");

    public:
        UpDownCounter(
            Clock& clock,
            Wire& enable,
            Wire& reset,
            Wire& direction,
            Bus<N>& count
        ) : clock_(clock),
            enable_(enable),
            reset_(reset),
            direction_(direction),
            count_(count),
            clock_wire_(clock.state()),
            r_adder_(count_, constant_one_, carry_in_, increment_next_, carry_out_),
            r_subs_(count_, constant_one_, borrow_in_, decrement_next_, borrow_out_),
            direction_mux_(decrement_next_, increment_next_, direction_, direction_next_),
            enable_mux_(count_, direction_next_, enable_, count_next_),
            reset_mux_(count_next_, zero_bus_, reset_, reg_input_),
            reg_(reg_input_, clock_wire_, count_)
        {
            constant_one_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                constant_one_[i].write(LogicState::LOW);
            }
            carry_in_.write(LogicState::LOW);
            borrow_in_.write(LogicState::LOW);
        }

        void evaluate() noexcept override
        {
            clock_wire_.write(clock_.state());

            // Setting up constant inputs
            constant_one_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                constant_one_[i].write(LogicState::LOW);
            }
            carry_in_.write(LogicState::LOW);
            borrow_in_.write(LogicState::LOW);

            // computing count + 1 using ripple carry adder
            r_adder_.evaluate();

            // computing count - 1 using ripple borrow subtractor
            r_subs_.evaluate();

            direction_mux_.evaluate();
            enable_mux_.evaluate();
            reset_mux_.evaluate();

            reg_.evaluate();
        }

    private:
        Clock& clock_;
        Wire& enable_;
        Wire& reset_;
        Wire& direction_;
        Bus<N>& count_;

        Wire clock_wire_;
        Wire carry_in_{LogicState::LOW};
        Wire carry_out_{LogicState::LOW};
        Wire borrow_in_{LogicState::LOW};
        Wire borrow_out_{LogicState::LOW};

        Bus<N> constant_one_;
        Bus<N> zero_bus_;
        Bus<N> decrement_next_;
        Bus<N> increment_next_;
        Bus<N> count_next_;
        Bus<N> reg_input_;
        Bus<N> direction_next_;

        // Internal components
        RippleCarryAdder<N> r_adder_;
        RippleBorrowSubtractor<N> r_subs_;

        // Internal multiplexers
        Mux<N> direction_mux_;
        Mux<N> enable_mux_;
        Mux<N> reset_mux_;
        Register<N> reg_;
    };
}
