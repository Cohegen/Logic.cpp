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
#include "combinational/multiplexers/Mux2to1.hpp"
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
            direction_muxes_(make_direction_muxes(std::make_index_sequence<N>{})),
            enable_muxes_(make_enable_muxes(std::make_index_sequence<N>{})),
            reset_muxes_(make_reset_muxes(std::make_index_sequence<N>{})),
            reg_(reg_input_, clock_wire_, count_)
        {
            constant_one_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                constant_one_[i].write(LogicState::LOW);
            }
            carry_in_.write(LogicState::LOW);
            borrow_in_.write(LogicState::LOW);
            zero_wire_.write(LogicState::LOW);
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
            zero_wire_.write(LogicState::LOW);

            // computing count + 1 using ripple carry adder
            r_adder_.evaluate();

            // computing count - 1 using ripple borrow subtractor
            r_subs_.evaluate();

            for (std::size_t i = 0; i < N; ++i) {
                direction_muxes_[i].evaluate();
                enable_muxes_[i].evaluate();
                reset_muxes_[i].evaluate();
            }

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
        Wire zero_wire_{LogicState::LOW};

        Bus<N> constant_one_;
        Bus<N> decrement_next_;
        Bus<N> increment_next_;
        Bus<N> count_next_;
        Bus<N> reg_input_;
        Bus<N> direction_next_;

        // Internal components
        RippleCarryAdder<N> r_adder_;
        RippleBorrowSubtractor<N> r_subs_;

        // Internal multiplexers
        std::array<Mux2to1, N> direction_muxes_;
        std::array<Mux2to1, N> enable_muxes_;
        std::array<Mux2to1, N> reset_muxes_;
        Register<N> reg_;

        // Constructing the direction muxes
        template<std::size_t... I>
        std::array<Mux2to1, N> make_direction_muxes(std::index_sequence<I...>)
        {
            return {Mux2to1(decrement_next_[I], increment_next_[I], direction_, direction_next_[I])...};
        }

        // Constructing the enable muxes
        template<std::size_t... I>
        std::array<Mux2to1, N> make_enable_muxes(std::index_sequence<I...>)
        {
            return {Mux2to1(count_[I], direction_next_[I], enable_, count_next_[I])...};
        }

        // Constructing the reset muxes
        template<std::size_t... I>
        std::array<Mux2to1, N> make_reset_muxes(std::index_sequence<I...>)
        {
            return {Mux2to1(count_next_[I], zero_wire_, reset_, reg_input_[I])...};
        }
    };
}
