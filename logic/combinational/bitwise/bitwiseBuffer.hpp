/*
    BitwiseBuffer.hpp

    An N-bit buffer implemented using N BufferGate objects.
*/

#pragma once

#include <cstddef>
#include <vector>

#include "gates/BUFFER.hpp"
#include "signals/bus.hpp"
#include "simulator/Component.hpp"

namespace logic
{
    template <std::size_t N>
    class BitwiseBuffer:public Component
    {
    public:
        BitwiseBuffer(
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

        std::vector<BufferGate> gates_;
    };

} // namespace logic
