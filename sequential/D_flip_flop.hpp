#ifndef D_FLIP_FLOP_HPP
#define D_FLIP_FLOP_HPP

class D_flip_flop
{
private:
    bool D;
    bool CLK;
    bool previousCLK;
    bool Q;
    bool Q_bar;

public:
    D_flip_flop(bool d = false, bool clk = false)
        : D(d), CLK(clk), previousCLK(clk), Q(false), Q_bar(true)
    {
    }

    void setInputs(bool d, bool clk)
    {
        D = d;
        CLK = clk;
    }

    void update()
    {
        if (!previousCLK && CLK) {
            Q = D;
            Q_bar = !D;
        }

        previousCLK = CLK;
    }

    bool getQ() const
    {
        return Q;
    }

    bool getQ_bar() const
    {
        return Q_bar;
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