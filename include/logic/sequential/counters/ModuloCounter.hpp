/*
 * A synchronous N-bit modulo-M counter.
 *
 * Behavior:
 *  - Reset asserted  -> counter loads zero.
 *  - Enable HIGH (1) -> counter increments by one, wrapping to 0 when (count + 1) == modulo.
 *  - Enable LOW (0)  -> counter retains its current value.
 *
 * Datapath:
 *   Register (count) -> RippleCarryAdder (count + 1)
 *                    -> Comparator (compare count + 1 with modulo value)
 *                    -> Wrap MUXes (select 0 if count + 1 == modulo, else count + 1)
 *                    -> Enable MUXes (select wrapped value vs current count)
 *                    -> Reset MUXes (select zero vs enabled value)
 *                    -> Register
 */
#pragma once
#include <array>
#include <utility>
#include <cstddef>
#include "simulator/Component.hpp"
#include "combinational/adders/RippleCarryAdder.hpp"
#include "combinational/comparators/Comparator.hpp"
#include "combinational/multiplexers/Mux.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"

namespace logic {

    template<std::size_t N>
    class ModuloCounter : public Component {
        static_assert(N > 0, "ModuloCounter width must be greater than 0");

    public:
        ModuloCounter(
            Clock& clock,
            Wire& enable,
            Wire& reset,
            Bus<N>& modulo,
            Bus<N>& count
        ) : clock_(clock),
            enable_(enable),
            reset_(reset),
            modulo_(modulo),
            count_(count),
            clock_wire_(clock.state()),
            r_adder_(count_, constant_one_, carry_in_, incremented_val_, carry_out_),
            comp_(incremented_val_, modulo_, equal_, greater_, less_),
            wrap_mux_(incremented_val_, zero_bus_, equal_, wrapped_next_),
            enable_mux_(count_, wrapped_next_, enable_, count_next_),
            reset_mux_(count_next_, zero_bus_, reset_, reg_input_),
            reg_(reg_input_, clock_wire_, count_)
        {
            constant_one_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                constant_one_[i].write(LogicState::LOW);
                zero_bus_[i].write(LogicState::LOW);
            }
            zero_bus_[0].write(LogicState::LOW);
            carry_in_.write(LogicState::LOW);
        }

        void evaluate() noexcept override
        {
            clock_wire_.write(clock_.state());

            // Setting up constant inputs
            constant_one_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                constant_one_[i].write(LogicState::LOW);
                zero_bus_[i].write(LogicState::LOW);
            }
            zero_bus_[0].write(LogicState::LOW);
            carry_in_.write(LogicState::LOW);

            // computing count + 1 with ripple carry adder
            r_adder_.evaluate();

            // compare count + 1 with modulo value
            comp_.evaluate();

            // evaluate multiplexers
            wrap_mux_.evaluate();
            enable_mux_.evaluate();
            reset_mux_.evaluate();

            reg_.evaluate();
        }

    private:
        Clock& clock_;
        Wire& enable_;
        Wire& reset_;
        Bus<N>& modulo_;
        Bus<N>& count_;

        Wire clock_wire_;
        Wire carry_in_{LogicState::LOW};
        Wire carry_out_{LogicState::LOW};
        Wire equal_{LogicState::LOW};
        Wire greater_{LogicState::LOW};
        Wire less_{LogicState::LOW};

        Bus<N> constant_one_;
        Bus<N> zero_bus_;
        Bus<N> incremented_val_;
        Bus<N> wrapped_next_;
        Bus<N> count_next_;
        Bus<N> reg_input_;

        // Internal components
        RippleCarryAdder<N> r_adder_;
        Comparator<N> comp_;

        // Internal multiplexers
        Mux<N> wrap_mux_;
        Mux<N> enable_mux_;
        Mux<N> reset_mux_;
        Register<N> reg_;
    };
}