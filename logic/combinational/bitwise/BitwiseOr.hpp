/*
    BitwiseOr.hpp

    An N-bit bitwise OR implemented using N OrGate objects.
*/

#pragma once

#include <cstddef>
#include <vector>

#include "gates/OR.hpp"
#include "signals/bus.hpp"

namespace logic
{
    template <std::size_t N>
    class BitwiseOr
    {
    public:
        BitwiseOr(
            Bus<N>& input_a,
            Bus<N>& input_b,
            Bus<N>& output)
            : input_a_(input_a),
              input_b_(input_b),
              output_(output)
        {
            gates_.reserve(N);

            for (std::size_t i = 0; i < N; ++i)
            {
                gates_.emplace_back(
                    input_a_[i],
                    input_b_[i],
                    output_[i]
                );
            }
        }

        void evaluate() noexcept
        {
            for (auto& gate : gates_)
            {
                gate.evaluate();
            }
        }

    private:
        Bus<N>& input_a_;
        Bus<N>& input_b_;
        Bus<N>& output_;

        std::vector<OrGate> gates_;
    };

} // namespace logic
