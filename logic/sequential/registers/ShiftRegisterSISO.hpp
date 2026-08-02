/*
    ShiftRegisterSISO.hpp

    Serial-In Serial-Out (SISO) Shift Register.

    Hardware Architecture:

        Serial In
            │
            ▼
       +---------+
       |  DFF0   |
       +---------+
            │
            ▼
        stage[0]
            │
            ▼
       +---------+
       |  DFF1   |
       +---------+
            │
            ▼
        stage[1]
            │
            ▼
           ...
            │
            ▼
       +---------+
       | DFFN-1  |
       +---------+
            │
            ▼
        Serial Out

    All D flip-flops share the same clock.
*/

#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "sequential/flipflops/D_flip_flop.hpp"
#include "signals/wire.hpp"

namespace logic
{

template <std::size_t N>
class ShiftRegisterSISO
{
    static_assert(N > 0, "Shift register must contain at least one flip-flop.");

public:
    ShiftRegisterSISO(
        Wire& serial_in,
        Wire& serial_out,
        Wire& clock)
        : serial_in_(serial_in),
          serial_out_(serial_out),
          clock_(clock)
    {
        flip_flops_.reserve(N);

        
        // Special case: 1-bit shift register
        // Serial In ---> DFF ---> Serial Out
        if constexpr (N == 1)
        {
            flip_flops_.emplace_back(
                serial_in_,
                clock_,
                serial_out_,
                q_bar_wires_[0]
            );
        }
        else
        {
            
            // First stage
            // Serial In ---> DFF0 ---> stage_wires_[0]
           

            flip_flops_.emplace_back(
                serial_in_,
                clock_,
                stage_wires_[0],
                q_bar_wires_[0]
            );

            // Middle stages
            // stage[i-1] ---> DFFi ---> stage[i]
         

            for (std::size_t i = 1; i < N - 1; ++i)
            {
                flip_flops_.emplace_back(
                    stage_wires_[i - 1],
                    clock_,
                    stage_wires_[i],
                    q_bar_wires_[i]
                );
            }

            
            // Final stage
            // stage[N-2] ---> DFF ---> Serial Out

            flip_flops_.emplace_back(
                stage_wires_[N - 2],
                clock_,
                serial_out_,
                q_bar_wires_[N - 1]
            );
        }
    }

    void evaluate() noexcept
    {
        /*NOTE:
        Each flip-flop is evaluated sequentially.
        A future event-driven simulator will update all
         flip-flops simultaneously on the clock edge
        */
        for (auto& flip_flop : flip_flops_)
        {
            flip_flop.evaluate();
        }
    }

private:
    
    // External connections
    Wire& serial_in_;
    Wire& serial_out_;
    Wire& clock_;

    
    // Internal wires

    // Connect consecutive flip-flops together.
    std::array<Wire, (N > 1 ? N - 1 : 0)> stage_wires_;

    // Complementary outputs (Q̅) of each flip-flop.
    std::array<Wire, N> q_bar_wires_;

    
    // Hardware
    std::vector<DFlipFlop> flip_flops_;
};

} 