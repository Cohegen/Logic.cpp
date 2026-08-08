/*
An implementation of a Johnson counter
*/
#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include "simulator/Component.hpp"
#include "gates/NOT.hpp"
#include "combinational/multiplexers/Mux2to1.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "sequential/registers/register.hpp"
#include "signals/clock.hpp"

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
            zero_wire_(LogicState::LOW),
            feedback_(LogicState::LOW),
            shifted_count_(),
            count_next_(),
            reg_input_(),
            feedback_not_(count_[N - 1], feedback_),
            enable_muxes_(make_enable_muxes(std::make_index_sequence<N>{})),
            reset_muxes_(make_reset_muxes(std::make_index_sequence<N>{})),
            reg_(reg_input_, clock_wire_, count_)
        {
            zero_wire_.write(LogicState::LOW);
            feedback_.write(LogicState::LOW);
        }

        void evaluate() noexcept override
        {
             // Sync internal clock wire state
             clock_wire_.write(clock_.state());
             zero_wire_.write(LogicState::LOW);

             feedback_not_.evaluate();

             shifted_count_[0].write(feedback_.read());

             for(std::size_t i=1;i<N;++i)
             {
                shifted_count_[i].write(count_[i-1].read());
             }

             for (std::size_t i = 0; i < N; ++i) {
                enable_muxes_[i].evaluate();
                reset_muxes_[i].evaluate();
            }

            reg_.evaluate();
        }

        private:
        Clock& clock_;
        Wire& enable_;
        Wire& reset_;
        Bus<N>& count_;

        // Internal wires and signals
        Wire clock_wire_;
        Wire zero_wire_{LogicState::LOW};
        Wire feedback_{LogicState::LOW};

        Bus<N> shifted_count_;
        Bus<N> count_next_;
        Bus<N> reg_input_;

        // Internal hardware components
        NotGate feedback_not_;
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
            return {Mux2to1(count_next_[I], zero_wire_, reset_, reg_input_[I])...};
        }

    };
        
}
