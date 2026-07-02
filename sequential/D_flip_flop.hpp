#ifndef D_FLIP_FLOP_HPP
#define D_FLIP_FLOP_HPP

#include <bitset>

class D_flip_flop
{
private:
    std::bitset<2> inputs; // bit 0 = D, bit 1 = CLK
    bool previousCLK;
    std::bitset<2> state; // bit 0 = Q, bit 1 = Q_bar

public:
    D_flip_flop(bool d = false, bool clk = false)
        : previousCLK(clk), state{0b10}
    {
        inputs[0] = d;
        inputs[1] = clk;
    }

    explicit D_flip_flop(const std::bitset<2>& in)
        : inputs(in), previousCLK(in[1]), state{0b10}
    {
    }

    void setInputs(bool d, bool clk)
    {
        inputs[0] = d;
        inputs[1] = clk;
    }

    void setInputs(const std::bitset<2>& in)
    {
        inputs = in;
    }

    void update()
    {
        const bool D = inputs[0];
        const bool CLK = inputs[1];

        if (!previousCLK && CLK) {
            state[0] = D;
            state[1] = !D;
        }

        previousCLK = CLK;
    }

    bool getD() const
    {
        return inputs[0];
    }

    bool getCLK() const
    {
        return inputs[1];
    }

    bool getQ() const
    {
        return state[0];
    }

    bool getQ_bar() const
    {
        return state[1];
    }

    std::bitset<2> outputs() const
    {
        return state;
    }
};

#endif
