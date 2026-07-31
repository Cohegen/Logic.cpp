/*
    ALU.hpp

    An N-bit arithmetic logic unit.
*/

#pragma once

#include <cstddef>

#include "signals/bus.hpp"
#include "signals/logicState.hpp"
#include "signals/wire.hpp"

namespace logic
{

template <std::size_t N>
class ALU
{
public:
    ALU(Bus<N>& a,
        Bus<N>& b,
        Bus<3>& opcode,
        Bus<N>& result,
        Wire& zero,
        Wire& carry)
        : a_(a),
          b_(b),
          opcode_(opcode),
          result_(result),
          zero_(zero),
          carry_(carry)
    {
    }

    void evaluate()
    {
        const LogicState op0 = opcode_[0].read();
        const LogicState op1 = opcode_[1].read();
        const LogicState op2 = opcode_[2].read();

        if (!is_binary(op0) || !is_binary(op1) || !is_binary(op2))
        {
            write_unknown();
            return;
        }

        const int opcode = (to_bool(op2) ? 4 : 0)
                         | (to_bool(op1) ? 2 : 0)
                         | (to_bool(op0) ? 1 : 0);

        switch (opcode)
        {
            case 0:
                evaluate_add();
                break;
            case 1:
                evaluate_subtract();
                break;
            case 2:
                evaluate_bitwise(logic_and);
                break;
            case 3:
                evaluate_bitwise(logic_or);
                break;
            case 4:
                evaluate_bitwise(logic_xor);
                break;
            case 5:
                evaluate_not_a();
                break;
            case 6:
                copy_bus(a_);
                carry_.write(LogicState::LOW);
                update_zero_flag();
                break;
            case 7:
                copy_bus(b_);
                carry_.write(LogicState::LOW);
                update_zero_flag();
                break;
        }
    }

private:
    using BinaryLogic = LogicState (*)(LogicState, LogicState);

    void evaluate_add()
    {
        LogicState carry = LogicState::LOW;

        for (std::size_t i = 0; i < N; ++i)
        {
            const LogicState a = a_[i].read();
            const LogicState b = b_[i].read();

            if (!is_binary(a) || !is_binary(b) || !is_binary(carry))
            {
                result_[i].write(LogicState::UNKNOWN);
                carry = LogicState::UNKNOWN;
                continue;
            }

            const bool abit = to_bool(a);
            const bool bbit = to_bool(b);
            const bool cbit = to_bool(carry);
            const bool sum = abit != bbit != cbit;
            const bool nextCarry = (abit && bbit) || (abit && cbit) || (bbit && cbit);

            result_[i].write(from_bool(sum));
            carry = from_bool(nextCarry);
        }

        carry_.write(carry);
        update_zero_flag();
    }

    void evaluate_subtract()
    {
        LogicState borrow = LogicState::LOW;

        for (std::size_t i = 0; i < N; ++i)
        {
            const LogicState a = a_[i].read();
            const LogicState b = b_[i].read();

            if (!is_binary(a) || !is_binary(b) || !is_binary(borrow))
            {
                result_[i].write(LogicState::UNKNOWN);
                borrow = LogicState::UNKNOWN;
                continue;
            }

            const bool abit = to_bool(a);
            const bool bbit = to_bool(b);
            const bool borrowBit = to_bool(borrow);
            const bool difference = abit != bbit != borrowBit;
            const bool nextBorrow = (!abit && (bbit || borrowBit)) || (bbit && borrowBit);

            result_[i].write(from_bool(difference));
            borrow = from_bool(nextBorrow);
        }

        carry_.write(borrow);
        update_zero_flag();
    }

    void evaluate_bitwise(BinaryLogic operation)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            result_[i].write(operation(a_[i].read(), b_[i].read()));
        }

        carry_.write(LogicState::LOW);
        update_zero_flag();
    }

    void evaluate_not_a()
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            result_[i].write(logic_not(a_[i].read()));
        }

        carry_.write(LogicState::LOW);
        update_zero_flag();
    }

    void copy_bus(Bus<N>& source)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            result_[i].write(source[i].read());
        }
    }

    void update_zero_flag()
    {
        LogicState zero = LogicState::HIGH;

        for (std::size_t i = 0; i < N; ++i)
        {
            const LogicState bit = result_[i].read();

            if (bit == LogicState::HIGH)
            {
                zero = LogicState::LOW;
                break;
            }

            if (!is_binary(bit))
            {
                zero = LogicState::UNKNOWN;
            }
        }

        zero_.write(zero);
    }

    void write_unknown()
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            result_[i].write(LogicState::UNKNOWN);
        }

        zero_.write(LogicState::UNKNOWN);
        carry_.write(LogicState::UNKNOWN);
    }

    Bus<N>& a_;
    Bus<N>& b_;
    Bus<3>& opcode_;
    Bus<N>& result_;
    Wire& zero_;
    Wire& carry_;
};

} 
