#pragma once

#include "signals/bus.hpp"
#include "signals/wire.hpp"
#include "simulator/Component.hpp"
#include <cstddef>

namespace logic
{

/**
 * @brief Logical Left Shifter (LLS)
 * Shifts input data left by `shift_amount` bits, filling low-order bits with LOW (0).
 */
template <std::size_t Width>
class LogicalLeftShifter : public Component
{
public:
    LogicalLeftShifter(Bus<Width>& in, Bus<Width>& shift_amount, Bus<Width>& out)
        : m_in(in), m_shiftAmount(shift_amount), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        std::size_t amount = 0;
        std::size_t shift_bus_size = m_shiftAmount.size();
        for (std::size_t i = 0; i < shift_bus_size; ++i)
        {
            if (m_shiftAmount[i].read() == LogicState::HIGH)
            {
                amount |= (static_cast<std::size_t>(1) << i);
            }
        }

        for (std::size_t i = 0; i < Width; ++i)
        {
            if (i >= amount)
            {
                m_out[i].write(m_in[i - amount].read());
            }
            else
            {
                m_out[i].write(LogicState::LOW);
            }
        }
    }

private:
    Bus<Width>& m_in;
    Bus<Width>& m_shiftAmount;
    Bus<Width>& m_out;
};

/**
 * @brief Logical Right Shifter (LRS)
 * Shifts input data right by `shift_amount` bits, filling high-order bits with LOW (0).
 */
template <std::size_t Width>
class LogicalRightShifter : public Component
{
public:
    LogicalRightShifter(Bus<Width>& in, Bus<Width>& shift_amount, Bus<Width>& out)
        : m_in(in), m_shiftAmount(shift_amount), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        std::size_t amount = 0;
        std::size_t shift_bus_size = m_shiftAmount.size();
        for (std::size_t i = 0; i < shift_bus_size; ++i)
        {
            if (m_shiftAmount[i].read() == LogicState::HIGH)
            {
                amount |= (static_cast<std::size_t>(1) << i);
            }
        }

        for (std::size_t i = 0; i < Width; ++i)
        {
            if (i + amount < Width)
            {
                m_out[i].write(m_in[i + amount].read());
            }
            else
            {
                m_out[i].write(LogicState::LOW);
            }
        }
    }

private:
    Bus<Width>& m_in;
    Bus<Width>& m_shiftAmount;
    Bus<Width>& m_out;
};

/**
 * @brief Arithmetic Right Shifter (ARS)
 * Shifts input data right by `shift_amount` bits, preserving MSB sign bit in high-order positions.
 */
template <std::size_t Width>
class ArithmeticRightShifter : public Component
{
public:
    ArithmeticRightShifter(Bus<Width>& in, Bus<Width>& shift_amount, Bus<Width>& out)
        : m_in(in), m_shiftAmount(shift_amount), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        std::size_t amount = 0;
        std::size_t shift_bus_size = m_shiftAmount.size();
        for (std::size_t i = 0; i < shift_bus_size; ++i)
        {
            if (m_shiftAmount[i].read() == LogicState::HIGH)
            {
                amount |= (static_cast<std::size_t>(1) << i);
            }
        }

        LogicState sign_bit = m_in[Width - 1].read();

        for (std::size_t i = 0; i < Width; ++i)
        {
            if (i + amount < Width)
            {
                m_out[i].write(m_in[i + amount].read());
            }
            else
            {
                m_out[i].write(sign_bit);
            }
        }
    }

private:
    Bus<Width>& m_in;
    Bus<Width>& m_shiftAmount;
    Bus<Width>& m_out;
};

/**
 * @brief Barrel Shifter
 * Flexible shift unit supporting Logical Left (00), Logical Right (01), Arithmetic Right (10), and Rotate Right (11).
 */
template <std::size_t Width, std::size_t ShiftBits>
class BarrelShifter : public Component
{
public:
    BarrelShifter(Bus<Width>& in, Bus<ShiftBits>& shift_amount, Bus<2>& mode, Bus<Width>& out)
        : m_in(in), m_shiftAmount(shift_amount), m_mode(mode), m_out(out)
    {
    }

    void evaluate() noexcept override
    {
        std::size_t amount = 0;
        for (std::size_t i = 0; i < ShiftBits; ++i)
        {
            if (m_shiftAmount[i].read() == LogicState::HIGH)
            {
                amount |= (static_cast<std::size_t>(1) << i);
            }
        }
        amount %= Width;

        std::size_t mode_val = 0;
        if (m_mode[0].read() == LogicState::HIGH) mode_val |= 1;
        if (m_mode[1].read() == LogicState::HIGH) mode_val |= 2;

        LogicState sign_bit = m_in[Width - 1].read();

        for (std::size_t i = 0; i < Width; ++i)
        {
            switch (mode_val)
            {
            case 0: // Logical Left Shift
                if (i >= amount)
                    m_out[i].write(m_in[i - amount].read());
                else
                    m_out[i].write(LogicState::LOW);
                break;

            case 1: // Logical Right Shift
                if (i + amount < Width)
                    m_out[i].write(m_in[i + amount].read());
                else
                    m_out[i].write(LogicState::LOW);
                break;

            case 2: // Arithmetic Right Shift
                if (i + amount < Width)
                    m_out[i].write(m_in[i + amount].read());
                else
                    m_out[i].write(sign_bit);
                break;

            case 3: // Rotate Right
                m_out[i].write(m_in[(i + amount) % Width].read());
                break;
            }
        }
    }

private:
    Bus<Width>& m_in;
    Bus<ShiftBits>& m_shiftAmount;
    Bus<2>& m_mode;
    Bus<Width>& m_out;
};

} // namespace logic
