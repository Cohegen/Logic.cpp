/*
    BitwiseXor.hpp

    An N-bit bitwise XOR implemented using N XorGate objects.
*/

#pragma once

#include <cstddef>
#include <vector>

#include "gates/XOR.hpp"
#include "signals/bus.hpp"

namespace logic
{
    template <std::size_t N>
    class BitwiseXor
    {
    public:
        BitwiseXor(
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

        std::vector<XorGate> gates_;
    };

} // namespace logic
