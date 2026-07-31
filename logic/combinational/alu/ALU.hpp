/*
    ALU.hpp

    An N-bit arithmetic logic unit built from reusable logic components.
*/

#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "combinational/adders/FullAdder.hpp"
#include "combinational/bitwise/BitwiseOr.hpp"
#include "combinational/bitwise/bitwiseAnd.hpp"
#include "combinational/bitwise/bitwiseBuffer.hpp"
#include "combinational/bitwise/bitwiseNot.hpp"
#include "combinational/bitwise/bitwiseXor.hpp"
#include "combinational/multiplexers/Mux8to1.hpp"
#include "combinational/subtractors/FullSubtractor.hpp"
#include "gates/NOT.hpp"
#include "gates/OR.hpp"
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
          carry_(carry),
          and_unit_(a_, b_, and_bus_),
          or_unit_(a_, b_, or_bus_),
          xor_unit_(a_, b_, xor_bus_),
          not_unit_(a_, not_bus_),
          pass_a_unit_(a_, pass_a_bus_),
          pass_b_unit_(b_, pass_b_bus_)
    {
        build_adders();
        build_subtractors();
        build_result_muxes();
        build_carry_mux();
        build_zero_detector();
    }

    void evaluate()
    {
        low_.write(LogicState::LOW);
        add_carry_in_.write(LogicState::LOW);
        sub_borrow_in_.write(LogicState::LOW);

        for (auto& adder : adders_)
        {
            adder->evaluate();
        }

        for (auto& subtractor : subtractors_)
        {
            subtractor->evaluate();
        }

        and_unit_.evaluate();
        or_unit_.evaluate();
        xor_unit_.evaluate();
        not_unit_.evaluate();
        pass_a_unit_.evaluate();
        pass_b_unit_.evaluate();

        for (auto& mux : result_muxes_)
        {
            mux->evaluate();
        }

        carry_mux_->evaluate();
        evaluate_zero_detector();
    }

private:
    void build_adders()
    {
        adders_.reserve(N);

        for (std::size_t i = 0; i < N; ++i)
        {
            Wire& carry_in = (i == 0) ? add_carry_in_ : add_carries_[i - 1];
            Wire& carry_out = (i == N - 1) ? add_carry_out_ : add_carries_[i];

            adders_.push_back(std::make_unique<FullAdder>(
                a_[i],
                b_[i],
                carry_in,
                add_bus_[i],
                carry_out));
        }
    }

    void build_subtractors()
    {
        subtractors_.reserve(N);

        for (std::size_t i = 0; i < N; ++i)
        {
            Wire& borrow_in = (i == 0) ? sub_borrow_in_ : sub_borrows_[i - 1];
            Wire& borrow_out = (i == N - 1) ? sub_borrow_out_ : sub_borrows_[i];

            subtractors_.push_back(std::make_unique<FullSubtractor>(
                a_[i],
                b_[i],
                borrow_in,
                sub_bus_[i],
                borrow_out));
        }
    }

    void build_result_muxes()
    {
        result_muxes_.reserve(N);

        for (std::size_t i = 0; i < N; ++i)
        {
            result_muxes_.push_back(std::make_unique<Mux8to1>(
                add_bus_[i],
                sub_bus_[i],
                and_bus_[i],
                or_bus_[i],
                xor_bus_[i],
                not_bus_[i],
                pass_a_bus_[i],
                pass_b_bus_[i],
                opcode_[0],
                opcode_[1],
                opcode_[2],
                result_[i]));
        }
    }

    void build_carry_mux()
    {
        carry_mux_ = std::make_unique<Mux8to1>(
            add_carry_out_,
            sub_borrow_out_,
            low_,
            low_,
            low_,
            low_,
            low_,
            low_,
            opcode_[0],
            opcode_[1],
            opcode_[2],
            carry_);
    }

    void build_zero_detector()
    {
        if constexpr (N == 1)
        {
            zero_not_ = std::make_unique<NotGate>(result_[0], zero_);
        }
        else
        {
            zero_or_wires_.resize(N - 1);
            zero_or_gates_.reserve(N - 1);

            zero_or_gates_.push_back(std::make_unique<OrGate>(
                result_[0],
                result_[1],
                zero_or_wires_[0]));

            for (std::size_t i = 2; i < N; ++i)
            {
                zero_or_gates_.push_back(std::make_unique<OrGate>(
                    zero_or_wires_[i - 2],
                    result_[i],
                    zero_or_wires_[i - 1]));
            }

            zero_not_ = std::make_unique<NotGate>(zero_or_wires_[N - 2], zero_);
        }
    }

    void evaluate_zero_detector()
    {
        for (auto& gate : zero_or_gates_)
        {
            gate->evaluate();
        }

        zero_not_->evaluate();
    }

    Bus<N>& a_;
    Bus<N>& b_;
    Bus<3>& opcode_;
    Bus<N>& result_;
    Wire& zero_;
    Wire& carry_;

    Wire low_;
    Wire add_carry_in_;
    Wire add_carry_out_;
    Wire sub_borrow_in_;
    Wire sub_borrow_out_;
    std::vector<Wire> add_carries_{N > 1 ? N - 1 : 0};
    std::vector<Wire> sub_borrows_{N > 1 ? N - 1 : 0};

    Bus<N> add_bus_;
    Bus<N> sub_bus_;
    Bus<N> and_bus_;
    Bus<N> or_bus_;
    Bus<N> xor_bus_;
    Bus<N> not_bus_;
    Bus<N> pass_a_bus_;
    Bus<N> pass_b_bus_;

    BitwiseAnd<N> and_unit_;
    BitwiseOr<N> or_unit_;
    BitwiseXor<N> xor_unit_;
    BitwiseNot<N> not_unit_;
    BitwiseBuffer<N> pass_a_unit_;
    BitwiseBuffer<N> pass_b_unit_;

    std::vector<std::unique_ptr<FullAdder>> adders_;
    std::vector<std::unique_ptr<FullSubtractor>> subtractors_;
    std::vector<std::unique_ptr<Mux8to1>> result_muxes_;
    std::unique_ptr<Mux8to1> carry_mux_;

    std::vector<Wire> zero_or_wires_;
    std::vector<std::unique_ptr<OrGate>> zero_or_gates_;
    std::unique_ptr<NotGate> zero_not_;
};

} 
