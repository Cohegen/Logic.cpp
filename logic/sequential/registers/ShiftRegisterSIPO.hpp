/*
    ShiftRegisterSIPO.hpp

    Serial-In Parallel-Out (SIPO) Shift Register.

    Hardware Architecture:

        Serial In
            │
            ▼
       +---------+
       |  DFF0   |──────────────► Parallel Out[0]
       +---------+
            │
            ▼
       +---------+
       |  DFF1   |──────────────► Parallel Out[1]
       +---------+
            │
            ▼
       +---------+
       |  DFF2   |──────────────► Parallel Out[2]
       +---------+
            │
            ▼
             ...
            │
            ▼
       +-----------+
       | DFF(N-1)  |────────────► Parallel Out[N-1]
       +-----------+

    Each D flip-flop shares the same clock.

    The parallel output bus is also used as the inter-stage
    connection between consecutive flip-flops.
*/

#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "sequential/flipflops/D_flip_flop.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

template <std::size_t N>
class ShiftRegisterSIPO:public Component
{
    static_assert(
        N > 0,
        "Shift register must contain at least one flip-flop."
    );

public:
    ShiftRegisterSIPO(
        Wire& serial_in,
        Wire& clock,
        Bus<N>& parallel_out)
        : serial_in_(serial_in),
          clock_(clock),
          parallel_out_(parallel_out)
    {
        flip_flops_.reserve(N);

       
        // Special case:
        // Single-bit shift register
        if constexpr (N == 1)
        {
            flip_flops_.emplace_back(
                serial_in_,
                clock_,
                parallel_out_[0],
                q_bar_wires_[0]
            );
        }
        else
        {
            //------------------------------------------------------
            // First stage
            //
            // Serial In ---> DFF0 ---> ParallelOut[0]
            //------------------------------------------------------

            flip_flops_.emplace_back(
                serial_in_,
                clock_,
                parallel_out_[0],
                q_bar_wires_[0]
            );

            //------------------------------------------------------
            // Middle stages
            //
            // ParallelOut[i-1]
            //          │
            //          ▼
            //        DFF(i)
            //          │
            //          ▼
            // ParallelOut[i]
            //------------------------------------------------------

            for (std::size_t i = 1; i < N - 1; ++i)
            {
                flip_flops_.emplace_back(
                    parallel_out_[i - 1],
                    clock_,
                    parallel_out_[i],
                    q_bar_wires_[i]
                );
            }

            
            // Final stage
           flip_flops_.emplace_back(
                parallel_out_[N - 2],
                clock_,
                parallel_out_[N - 1],
                q_bar_wires_[N - 1]
            );
        }
    }

    void evaluate() noexcept
    {
        // NOTE:
        // Flip-flops are evaluated sequentially.
        //
        // A future event-driven simulator will update all
        // flip-flops simultaneously on the active clock edge.

        for (auto& flip_flop : flip_flops_)
        {
            flip_flop.evaluate();
        }
    }

private:
   
    // Inputs
    Wire& serial_in_;
    Wire& clock_;

   
    // Outputs
    Bus<N>& parallel_out_;

   
    // Internal wires
    // Complementary outputs (Q̅) of each D flip-flop.
    std::array<Wire, N> q_bar_wires_;

   
    // Hardware
    std::vector<DFlipFlop> flip_flops_;
};

}