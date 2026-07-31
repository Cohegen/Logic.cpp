/*
    bus.hpp

    A Bus represents a collection of wires.

    In digital electronics, a bus is simply multiple wires
    grouped together to carry multi-bit values between
    components.

    Example:
        Bus<8> dataBus;     // 8-bit data bus
        Bus<16> addressBus; // 16-bit address bus
*/

#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>

#include "signals/wire.hpp"

namespace logic
{

template <std::size_t N>
class Bus
{
    static_assert(N > 0, "Bus width must be greater than zero.");

public:
    using value_type      = Wire;
    using size_type       = std::size_t;
    using iterator        = typename std::array<Wire, N>::iterator;
    using const_iterator  = typename std::array<Wire, N>::const_iterator;

    /// Default constructor
    Bus() = default;

    /// Number of wires in the bus
    [[nodiscard]]
    constexpr size_type size() const noexcept
    {
        return N;
    }

    /// Is the bus empty?
    [[nodiscard]]
    constexpr bool empty() const noexcept
    {
        return N == 0;
    }

    /// Fast access (no bounds checking)
    [[nodiscard]]
    Wire& operator[](size_type index) noexcept
    {
        return m_wires[index];
    }

    [[nodiscard]]
    const Wire& operator[](size_type index) const noexcept
    {
        return m_wires[index];
    }

    /// Bounds-checked access
    [[nodiscard]]
    Wire& at(size_type index)
    {
        if (index >= N)
        {
            throw std::out_of_range("Bus index out of range.");
        }

        return m_wires[index];
    }

    [[nodiscard]]
    const Wire& at(size_type index) const
    {
        if (index >= N)
        {
            throw std::out_of_range("Bus index out of range.");
        }

        return m_wires[index];
    }

    /// First wire
    [[nodiscard]]
    Wire& front() noexcept
    {
        return m_wires.front();
    }

    [[nodiscard]]
    const Wire& front() const noexcept
    {
        return m_wires.front();
    }

    /// Last wire
    [[nodiscard]]
    Wire& back() noexcept
    {
        return m_wires.back();
    }

    [[nodiscard]]
    const Wire& back() const noexcept
    {
        return m_wires.back();
    }

    /// Iterators
    [[nodiscard]]
    iterator begin() noexcept
    {
        return m_wires.begin();
    }

    [[nodiscard]]
    const_iterator begin() const noexcept
    {
        return m_wires.begin();
    }

    [[nodiscard]]
    const_iterator cbegin() const noexcept
    {
        return m_wires.cbegin();
    }

    [[nodiscard]]
    iterator end() noexcept
    {
        return m_wires.end();
    }

    [[nodiscard]]
    const_iterator end() const noexcept
    {
        return m_wires.end();
    }

    [[nodiscard]]
    const_iterator cend() const noexcept
    {
        return m_wires.cend();
    }

    /// Set every wire to the same value
    void write(LogicState state)
    {
        for (auto& wire : m_wires)
        {
            wire.write(state);
        }
    }

    /// Reset all wires to LOW
    void clear()
    {
        write(LogicState::LOW);
    }

private:
    std::array<Wire, N> m_wires;
};

}
