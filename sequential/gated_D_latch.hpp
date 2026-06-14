#ifndef GATED_D_LATCH_HPP
#define GATED_D_LATCH_HPP

#include "SR_latch.hpp"

class D_latch : public SR_Latch
{
protected:
    bool D; // input D
    bool CLK; // clock signal

public:
    D_latch(bool d = false, bool clk = false)
        : SR_Latch(d && clk, !d && clk), D(d), CLK(clk)
    {
    }

    void setInputs(bool d, bool clk) override
    {
        D = d;
        CLK = clk;
    }

    void update() override
    {
        SR_Latch::setInputs(D && CLK, !D && CLK);
        SR_Latch::update();
    }

    bool getD() const
    {
        return D;
    }

    bool getCLK() const
    {
        return CLK;
    }
};

#endif
