/*
An implementation of binary counter
*/

#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include "simulator/Component.hpp"
#include "combinational/adders/RippleCarryAdder.hpp"
#include "combinational/multiplexers/Mux2to1.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"

namespace logic {
    template<std::size_t N>
    class BinaryCounter : public Component {
        static_assert(N > 0, "BinaryCounter width must be greater than zero.");

    public:
        BinaryCounter(
            Clock& clock,
            Wire& enable,
            Wire& reset,
            Bus<N>& count
        ) : clock_(clock),
            enable_(enable),
            reset_(reset),
            count_(count),
            clock_wire_(clock.state()),
            r_adder_(count_, constant_one_, carry_in_, adder_next_, carry_out_),
            enable_muxes_(make_enable_muxes(std::make_index_sequence<N>{})),
            reset_muxes_(make_reset_muxes(std::make_index_sequence<N>{})),
            reg_(reg_input_, clock_wire_, count_)
        {
            // Set up constant 1 for adder input
            constant_one_[0].write(LogicState::HIGH);
            for (std::size_t i = 1; i < N; ++i) {
                constant_one_[i].write(LogicState::LOW);
            }
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
            zero_wire_.write(LogicState::LOW);

            // Computing count + 1 using adder
            r_adder_.evaluate();

            // Passing outputs through enable and reset multiplexers
            for (std::size_t i = 0; i < N; ++i) {
                enable_muxes_[i].evaluate();
                reset_muxes_[i].evaluate();
            }

            // Clock update on register
            reg_.evaluate();
        }

    private:
        Clock& clock_;
        Wire& enable_;
        Wire& reset_;
        Bus<N>& count_;

        Wire clock_wire_;
        Wire carry_in_{LogicState::LOW};
        Wire carry_out_{LogicState::LOW};
        Wire zero_wire_{LogicState::LOW};

        Bus<N> constant_one_;
        Bus<N> adder_next_;
        Bus<N> count_next_;
        Bus<N> reg_input_;

        RippleCarryAdder<N> r_adder_;
        std::array<Mux2to1, N> enable_muxes_;
        std::array<Mux2to1, N> reset_muxes_;
        Register<N> reg_;

        template<std::size_t... I>
        std::array<Mux2to1, N> make_enable_muxes(std::index_sequence<I...>)
        {
            return {Mux2to1(count_[I], adder_next_[I], enable_, count_next_[I])...};
        }

        template<std::size_t... I>
        std::array<Mux2to1, N> make_reset_muxes(std::index_sequence<I...>)
        {
            return {Mux2to1(count_next_[I], zero_wire_, reset_, reg_input_[I])...};
        }
    };
}



