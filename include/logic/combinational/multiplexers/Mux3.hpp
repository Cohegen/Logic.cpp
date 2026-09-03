/*
    Mux3.hpp

    An N-bit 3-to-1 Multiplexer component built from N single-bit Mux3to1 gates.

    Inputs:
        a: Bus<N> (selected when select1 select0 is 00)
        b: Bus<N> (selected when select1 select0 is 01)
        c: Bus<N> (selected when select1 select0 is 10)
        select0: Wire (LSB select signal)
        select1: Wire (MSB select signal)
    Output:
        output: Bus<N>
*/

#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "combinational/multiplexers/Mux3to1.hpp"
#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"

namespace logic
{

template <std::size_t N>
class Mux3 : public Component
{
public:
    Mux3(Bus<N>& a, Bus<N>& b, Bus<N>& c, Wire& select0, Wire& select1, Bus<N>& output)
        : m_a(a), m_b(b), m_c(c), m_select0(select0), m_select1(select1), m_output(output)
    {
        m_muxes.reserve(N);
        for (std::size_t i = 0; i < N; ++i)
        {
            m_muxes.push_back(std::make_unique<Mux3to1>(
                m_a[i], m_b[i], m_c[i], m_select0, m_select1, m_output[i]
            ));
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
    Bus<N>& m_c;
    Wire& m_select0;
    Wire& m_select1;
    Bus<N>& m_output;

    std::vector<std::unique_ptr<Mux3to1>> m_muxes;
};

} // namespace logic
