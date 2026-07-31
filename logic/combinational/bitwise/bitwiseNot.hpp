/*
    BitwiseNot.hpp

    An N-bit bitwise NOT implemented using N NotGate objects.
*/

#pragma once

#include <cstddef>
#include <vector>

#include "gates/NOT.hpp"
#include "signals/bus.hpp"

namespace logic
{
    template <std::size_t N>
    class BitwiseNot
    {
    public:
        BitwiseNot(
            Bus<N>& input,
            Bus<N>& output)
            : input_(input),
              output_(output)
        {
            gates_.reserve(N);

            for (std::size_t i = 0; i < N; ++i)
            {
                gates_.emplace_back(
                    input_[i],
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
        Bus<N>& input_;
        Bus<N>& output_;

        std::vector<NotGate> gates_;
    };

} // namespace logic
