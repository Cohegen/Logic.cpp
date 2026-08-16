/*
    ShiftRegisterPISO.hpp

    Parallel-In Serial-Out (PISO) Shift Register.

    Hardware Architecture for an N-bit PISO Shift Register:
    
    For stage 0 (MSB / first stage):
        Multiplexer Mux0:
            Input 0 (shift input): serial_in
            Input 1 (load input):  parallel_in[0]
            Select:               load
            Output:               d_in_0
        D Flip-Flop DFF0:
            Data in:   d_in_0
            Clock:     clock
            Q out:     stage_wires_[0]

    For stage i (1 <= i < N - 1):
        Multiplexer Mux_i:
            Input 0 (shift input): stage_wires_[i-1]
            Input 1 (load input):  parallel_in[i]
            Select:               load
            Output:               d_in_i
        D Flip-Flop DFF_i:
            Data in:   d_in_i
            Clock:     clock
            Q out:     stage_wires_[i]

    For stage N-1 (LSB / final stage):
        Multiplexer Mux_{N-1}:
            Input 0 (shift input): stage_wires_[N-2]  (or serial_in if N=1)
            Input 1 (load input):  parallel_in[N-1]
            Select:               load
            Output:               d_in_{N-1}
        D Flip-Flop DFF_{N-1}:
            Data in:   d_in_{N-1}
            Clock:     clock
            Q out:     serial_out
*/

#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "sequential/flipflops/D_flip_flop.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "combinational/multiplexers/Mux.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "sequential/flipflops/D_flip_flop.hpp"
#include "simulator/Component.hpp"

namespace logic {

template <std::size_t N>
class ShiftRegisterPISO : public Component {
    static_assert(
        N > 0,
        "Shift register must contain at least one flip-flop."
    );

public:
    ShiftRegisterPISO(
        Bus<N>& parallel_in,
        Wire& serial_in,
        Wire& serial_out,
        Wire& clock,
        Wire& load
    )
        : parallel_in_(parallel_in),
          serial_in_(serial_in),
          serial_out_(serial_out),
          clock_(clock),
          load_(load),
          load_mux_(shift_bus_, parallel_in_, load_, d_in_bus_)
    {
        flip_flops_.reserve(N);

        if constexpr (N == 1) {
            flip_flops_.emplace_back(
                d_in_bus_[0],
                clock_,
                serial_out_,
                q_bar_wires_[0]
            );
        } else {
            // First stage (i = 0)
            flip_flops_.emplace_back(
                d_in_bus_[0],
                clock_,
                stage_wires_[0],
                q_bar_wires_[0]
            );

            // Middle stages (0 < i < N-1)
            for (std::size_t i = 1; i < N - 1; ++i) {
                flip_flops_.emplace_back(
                    d_in_bus_[i],
                    clock_,
                    stage_wires_[i],
                    q_bar_wires_[i]
                );
            }

            // Final stage (i = N - 1)
            flip_flops_.emplace_back(
                d_in_bus_[N - 1],
                clock_,
                serial_out_,
                q_bar_wires_[N - 1]
            );
        }
    }

    void evaluate() noexcept override {
        // Construct shift_bus_ inputs from serial_in and stage_wires
        shift_bus_[0].write(serial_in_.read());
        if constexpr (N > 1) {
            for (std::size_t i = 1; i < N; ++i) {
                shift_bus_[i].write(stage_wires_[i - 1].read());
            }
        }

        // Evaluate bus multiplexer to update DFF data inputs
        load_mux_.evaluate();

        // Evaluate all D flip-flops
        for (auto& flip_flop : flip_flops_) {
            flip_flop.evaluate();
        }
    }

private:
    // Inputs & Outputs
    Bus<N>& parallel_in_;
    Wire& serial_in_;
    Wire& serial_out_;
    Wire& clock_;
    Wire& load_; // 1 = Load parallel_in, 0 = Shift

    // Internal connecting signals
    Bus<N> shift_bus_;
    Bus<N> d_in_bus_;
    std::array<Wire, (N > 1 ? N - 1 : 0)> stage_wires_;
    std::array<Wire, N> q_bar_wires_;

    // Hardware components
    Mux<N> load_mux_;
    std::vector<DFlipFlop> flip_flops_;
};

} // namespace logic