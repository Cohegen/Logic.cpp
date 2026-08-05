/*
    Register.hpp

    An N-bit register implemented using N D flip-flops.

    Every flip-flop stores one bit of the input bus.
    All flip-flops share the same clock.
*/

#pragma once

#include <cstddef>
#include <vector>

#include "signals/bus.hpp"
#include "signals/wire.hpp"

#include "sequential/flipflops/D_flip_flop.hpp"
#include "simulator/Component.hpp"

namespace logic
{

template <std::size_t N>
class Register:public Component
{
public:
    Register(
        Bus<N>& input,
        Wire& clock,
        Bus<N>& output)
        : input_(input),
          clock_(clock),
          output_(output)
    {
        flip_flops_.reserve(N);

        for (std::size_t i = 0; i < N; ++i)
        {
            flip_flops_.emplace_back(
                input_[i],        // D input
                clock_,           // Shared clock
                output_[i],       // Q output
                output_bar_[i]    // Internal Q̅ output
            );
        }
    }

    void evaluate() noexcept
    {
        for (auto& flip_flop : flip_flops_)
        {
            flip_flop.evaluate();
        }
    }

private:
    
    // Inputs

    Bus<N>& input_;
    Wire& clock_;

    
    // Outputs
    Bus<N>& output_;

    
    // Internal signals
    // Complementary outputs (Q̅) from each flip-flop
    Bus<N> output_bar_;

    
    // internal hardware
   std::vector<DFlipFlop> flip_flops_;
};

} 