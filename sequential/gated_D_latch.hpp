#ifndef GATED_D_LATCH_HPP
#define GATED_D_LATCH_HPP

#include "SR_latch.hpp"
#include <bitset>

class D_latch : public SR_Latch
{
protected:
    std::bitset<2> dInputs; // bit 0 = D, bit 1 = CLK

public:
    D_latch(bool d = false, bool clk = false)
        : SR_Latch(d && clk, !d && clk)
    {
        dInputs[0] = d;
        dInputs[1] = clk;
    }

    explicit D_latch(const std::bitset<2>& in)
        : SR_Latch(in[0] && in[1], !in[0] && in[1]), dInputs(in)
    {
    }

    void setInputs(bool d, bool clk) override
    {
        dInputs[0] = d;
        dInputs[1] = clk;
    }

    void setInputs(const std::bitset<2>& in) override
    {
        dInputs = in;
    }

    void update() override
    {
        const bool D = dInputs[0];
        const bool CLK = dInputs[1];
        SR_Latch::setInputs(D && CLK, !D && CLK);
        SR_Latch::update();
    }

    bool getD() const
    {
        return dInputs[0];
    }

    bool getCLK() const
    {
        return dInputs[1];
    }
};

#endif
