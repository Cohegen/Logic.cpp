/*
An implementation of binary counter
*/

#pragma once

#include <cstddef>
#include <vector>

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
            reg_(reg_input_, clock_wire_, count_),
            r_adder_(count_, constant_one_, carry_in_, adder_next_, carry_out_)
        {
            muxes_.reserve(2 * N);
            for (std::size_t i = 0; i < N; ++i) {
                // Selects count_[i] when enable=0, adder_next_[i] when enable=1
                muxes_.emplace_back(count_[i], adder_next_[i], enable_, count_next_[i]);
                // Selects count_next_[i] when reset=0, LOW when reset=1
                muxes_.emplace_back(count_next_[i], zero_wire_, reset_, reg_input_[i]);
            }
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

            // Passing outputs through enable & reset multiplexers
            for (auto& mux : muxes_) {
                mux.evaluate();
            }

            // Clocking update on register
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
        std::vector<Mux2to1> muxes_;
        Register<N> reg_;
    };
}



