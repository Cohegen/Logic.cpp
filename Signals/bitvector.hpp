#pragma once

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

template <std::size_t N>
class BitVector
{
public:
    
    // Constructors
    BitVector() = default;

    explicit BitVector(std::uint64_t value) noexcept
        : m_vector(value)
    {
    }

    explicit BitVector(const std::string& bits)
        : m_vector(bits)
    {
    }

    explicit BitVector(std::string_view bits)
        : m_vector(std::string(bits))
    {
    }

    explicit BitVector(const std::bitset<N>& bits) noexcept
        : m_vector(bits)
    {
    }

    
    // Capacity
    

    [[nodiscard]]
    constexpr std::size_t size() const noexcept
    {
        return N;
    }

    
    // Element access
    

    [[nodiscard]]
    bool operator[](std::size_t index) const
    {
        check_index(index);
        return m_vector[index];
    }

    [[nodiscard]]
    bool test(std::size_t index) const
    {
        return (*this)[index];
    }

    
    // Modifiers
    

    void set(std::size_t index, bool value = true)
    {
        check_index(index);
        m_vector.set(index, value);
    }

    void reset(std::size_t index)
    {
        check_index(index);
        m_vector.reset(index);
    }

    void reset() noexcept
    {
        m_vector.reset();
    }

    void flip(std::size_t index)
    {
        check_index(index);
        m_vector.flip(index);
    }

    void flip() noexcept
    {
        m_vector.flip();
    }

    
    // Queries
    

    [[nodiscard]]
    bool any() const noexcept
    {
        return m_vector.any();
    }

    [[nodiscard]]
    bool none() const noexcept
    {
        return m_vector.none();
    }

    [[nodiscard]]
    bool all() const noexcept
    {
        return m_vector.all();
    }

    [[nodiscard]]
    std::size_t count() const noexcept
    {
        return m_vector.count();
    }

    
    // Conversion
    

    [[nodiscard]]
    unsigned long to_ulong() const
    {
        return m_vector.to_ulong();
    }

    [[nodiscard]]
    unsigned long long to_ullong() const
    {
        return m_vector.to_ullong();
    }

    [[nodiscard]]
    std::string to_string() const
    {
        return m_vector.to_string();
    }

    
    // Comparison

    [[nodiscard]]
    bool operator==(const BitVector& other) const noexcept
    {
        return m_vector == other.m_vector;
    }

    [[nodiscard]]
    bool operator!=(const BitVector& other) const noexcept
    {
        return !(*this == other);
    }

    
    // Bitwise operators
    

    [[nodiscard]]
    BitVector operator&(const BitVector& other) const noexcept
    {
        return BitVector(m_vector & other.m_vector);
    }

    [[nodiscard]]
    BitVector operator|(const BitVector& other) const noexcept
    {
        return BitVector(m_vector | other.m_vector);
    }

    [[nodiscard]]
    BitVector operator^(const BitVector& other) const noexcept
    {
        return BitVector(m_vector ^ other.m_vector);
    }

    [[nodiscard]]
    BitVector operator~() const noexcept
    {
        return BitVector(~m_vector);
    }

    [[nodiscard]]
    BitVector operator<<(std::size_t shift) const noexcept
    {
        return BitVector(m_vector << shift);
    }

    [[nodiscard]]
    BitVector operator>>(std::size_t shift) const noexcept
    {
        return BitVector(m_vector >> shift);
    }

    
    // Compound assignment
    

    BitVector& operator&=(const BitVector& other) noexcept
    {
        m_vector &= other.m_vector;
        return *this;
    }

    BitVector& operator|=(const BitVector& other) noexcept
    {
        m_vector |= other.m_vector;
        return *this;
    }

    BitVector& operator^=(const BitVector& other) noexcept
    {
        m_vector ^= other.m_vector;
        return *this;
    }

    BitVector& operator<<=(std::size_t shift) noexcept
    {
        m_vector <<= shift;
        return *this;
    }

    BitVector& operator>>=(std::size_t shift) noexcept
    {
        m_vector >>= shift;
        return *this;
    }

private:
    void check_index(std::size_t index) const
    {
        if (index >= N)
        {
            throw std::out_of_range("BitVector index out of range");
        }
    }

private:
    std::bitset<N> m_vector;
};