/*
    MuxTree.hpp

    A parameterized binary tree multiplexer (e.g. 32-to-1, 16-to-1, 4-to-1)
    composed of 2-input Mux<DataWidth> stages.

    For NumInputs = 32 and DataWidth = 128:
      Stage 0: 32 inputs -> 16 intermediate outputs (controlled by Select[0])
      Stage 1: 16 inputs ->  8 intermediate outputs (controlled by Select[1])
      Stage 2:  8 inputs ->  4 intermediate outputs (controlled by Select[2])
      Stage 3:  4 inputs ->  2 intermediate outputs (controlled by Select[3])
      Stage 4:  2 inputs ->  1 final output (controlled by Select[4])
      Total: 31 Mux<128> components.
*/

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "combinational/multiplexers/Mux.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

namespace detail
{
    constexpr std::size_t clog2(std::size_t val)
    {
        std::size_t bits = 0;
        while ((static_cast<std::size_t>(1) << bits) < val)
        {
            ++bits;
        }
        return bits;
    }
} // namespace detail

/**
 * @brief Multi-input Multiplexer Tree
 * 
 * @tparam NumInputs Number of input buses (must be a power of 2, >= 2)
 * @tparam DataWidth Bit-width of each bus
 * @tparam SelectBits Width of the select bus (defaults to ceil(log2(NumInputs)))
 */
template <
    std::size_t NumInputs,
    std::size_t DataWidth,
    std::size_t SelectBits = detail::clog2(NumInputs)
>
class MuxTree : public Component
{
    static_assert(NumInputs >= 2, "MuxTree requires at least 2 inputs.");
    static_assert((NumInputs & (NumInputs - 1)) == 0, "NumInputs must be a power of 2.");
    static_assert(SelectBits >= detail::clog2(NumInputs), "SelectBits is insufficient for NumInputs.");

public:
    /// Construct from std::array of Bus pointers
    MuxTree(
        const std::array<Bus<DataWidth>*, NumInputs>& inputs,
        Bus<SelectBits>& select,
        Bus<DataWidth>& output
    ) : m_select(select), m_output(output)
    {
        std::vector<Bus<DataWidth>*> input_ptrs(inputs.begin(), inputs.end());
        build_tree(input_ptrs);
    }

    /// Construct from std::vector of Bus pointers
    MuxTree(
        const std::vector<Bus<DataWidth>*>& inputs,
        Bus<SelectBits>& select,
        Bus<DataWidth>& output
    ) : m_select(select), m_output(output)
    {
        if (inputs.size() != NumInputs)
        {
            throw std::invalid_argument("MuxTree inputs size must match NumInputs.");
        }
        build_tree(inputs);
    }

    /// Construct from std::array of Bus references
    MuxTree(
        const std::array<std::reference_wrapper<Bus<DataWidth>>, NumInputs>& inputs,
        Bus<SelectBits>& select,
        Bus<DataWidth>& output
    ) : m_select(select), m_output(output)
    {
        std::vector<Bus<DataWidth>*> input_ptrs;
        input_ptrs.reserve(NumInputs);
        for (const auto& ref : inputs)
        {
            input_ptrs.push_back(&ref.get());
        }
        build_tree(input_ptrs);
    }

    /// Construct from std::vector of Bus references
    MuxTree(
        const std::vector<std::reference_wrapper<Bus<DataWidth>>>& inputs,
        Bus<SelectBits>& select,
        Bus<DataWidth>& output
    ) : m_select(select), m_output(output)
    {
        if (inputs.size() != NumInputs)
        {
            throw std::invalid_argument("MuxTree inputs size must match NumInputs.");
        }
        std::vector<Bus<DataWidth>*> input_ptrs;
        input_ptrs.reserve(NumInputs);
        for (const auto& ref : inputs)
        {
            input_ptrs.push_back(&ref.get());
        }
        build_tree(input_ptrs);
    }

    void evaluate() noexcept override
    {
        for (auto& mux : m_muxes)
        {
            mux->evaluate();
        }
    }

private:
    void build_tree(const std::vector<Bus<DataWidth>*>& initial_inputs)
    {
        std::vector<Bus<DataWidth>*> current_inputs = initial_inputs;
        constexpr std::size_t num_stages = detail::clog2(NumInputs);

        m_muxes.reserve(NumInputs - 1);
        if constexpr (NumInputs > 2)
        {
            m_intermediate_buses.reserve(NumInputs - 2);
        }

        for (std::size_t stage = 0; stage < num_stages; ++stage)
        {
            const std::size_t stage_mux_count = current_inputs.size() / 2;
            const bool is_final_stage = (stage == num_stages - 1);

            std::vector<Bus<DataWidth>*> next_inputs;
            next_inputs.reserve(stage_mux_count);

            for (std::size_t i = 0; i < stage_mux_count; ++i)
            {
                Bus<DataWidth>& inA = *current_inputs[2 * i];
                Bus<DataWidth>& inB = *current_inputs[2 * i + 1];
                Wire& sel = m_select[stage];

                if (is_final_stage)
                {
                    m_muxes.push_back(std::make_unique<Mux<DataWidth>>(inA, inB, sel, m_output));
                }
                else
                {
                    m_intermediate_buses.push_back(std::make_unique<Bus<DataWidth>>());
                    Bus<DataWidth>& outBus = *m_intermediate_buses.back();
                    next_inputs.push_back(&outBus);
                    m_muxes.push_back(std::make_unique<Mux<DataWidth>>(inA, inB, sel, outBus));
                }
            }

            current_inputs = std::move(next_inputs);
        }
    }

    Bus<SelectBits>& m_select;
    Bus<DataWidth>& m_output;

    std::vector<std::unique_ptr<Bus<DataWidth>>> m_intermediate_buses;
    std::vector<std::unique_ptr<Mux<DataWidth>>> m_muxes;
};

} // namespace logic
