/*
    8-to-1 Multiplexer

    Built hierarchically from:

        - Two Mux4to1
        - One Mux2to1

    Select Lines

        S2 S1 S0

    Truth Table

        S2 S1 S0 | Output
        -----------------
         0  0  0 | I0
         0  0  1 | I1
         0  1  0 | I2
         0  1  1 | I3
         1  0  0 | I4
         1  0  1 | I5
         1  1  0 | I6
         1  1  1 | I7
*/

#pragma once

#include "signals/wire.hpp"
#include "combinational/multiplexers/Mux4to1.hpp"
#include "combinational/multiplexers/Mux2to1.hpp"

namespace logic
{

class Mux8to1
{
public:

    Mux8to1(
        Wire& i0,
        Wire& i1,
        Wire& i2,
        Wire& i3,
        Wire& i4,
        Wire& i5,
        Wire& i6,
        Wire& i7,
        Wire& select0,
        Wire& select1,
        Wire& select2,
        Wire& output
    )
        :
        upper_mux(
            i0,
            i1,
            i2,
            i3,
            select0,
            select1,
            upper_output_
        ),
        lower_mux(
            i4,
            i5,
            i6,
            i7,
            select0,
            select1,
            lower_output_
        ),
        output_mux(
            upper_output_,
            lower_output_,
            select2,
            output
        )
    {
    }

    void evaluate()
    {
        // Evaluating lower-level multiplexers first
        upper_mux.evaluate();
        lower_mux.evaluate();

        // Selecting between the two intermediate outputs
        output_mux.evaluate();
    }

private:

    // Intermediate wires
    Wire upper_output_;
    Wire lower_output_;

    // Internal multiplexers
    Mux4to1 upper_mux;
    Mux4to1 lower_mux;
    Mux2to1 output_mux;
};

} // namespace logic