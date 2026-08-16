/*
    Mux.hpp

    An N-bit 2-to-1 Multiplexer component built from N single-bit Mux2to1 gate logic components.

    Inputs:
        a: Bus<N> (selected when select is 0 / LOW)
        b: Bus<N> (selected when select is 1 / HIGH)
        select: Wire (select signal)
    Output:
        output: Bus<N>
*/

#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "combinational/multiplexers/Mux2to1.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

template <std::size_t N>
class Mux : public Component
{
public:
    Mux(Bus<N>& a, Bus<N>& b, Wire& select, Bus<N>& output)
        : m_a(a), m_b(b), m_select(select), m_output(output)
    {
        m_muxes.reserve(N);
        for (std::size_t i = 0; i < N; ++i)
        {
            m_muxes.push_back(std::make_unique<Mux2to1>(m_a[i], m_b[i], m_select, m_output[i]));
        }
    }

    void evaluate() noexcept override
    {
        for (auto& mux : m_muxes)
        {
            mux->evaluate();
        }
    }

private:
    Bus<N>& m_a;
    Bus<N>& m_b;
    Wire& m_select;
    Bus<N>& m_output;

    std::vector<std::unique_ptr<Mux2to1>> m_muxes;
};

} // namespace logic
