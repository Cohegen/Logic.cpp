#ifndef SR_LATCH_HPP
#define SR_LATCH_HPP

#include <bitset>
#include <iostream>

class SR_Latch
{
protected:
    std::bitset<2> inputs;
    std::bitset<2> state;

public:
    SR_Latch(bool s = false, bool r = false)
        : state{0b10}
    {
        inputs[0] = s;
        inputs[1] = r;
        update();
    }

    explicit SR_Latch(const std::bitset<2>& in)
        : inputs(in), state{0b10}
    {
        update();
    }

    virtual ~SR_Latch() = default;

    virtual void setInputs(bool s, bool r)
    {
        inputs[0] = s;
        inputs[1] = r;
    }

    virtual void setInputs(const std::bitset<2>& in)
    {
        inputs = in;
    }

    virtual void update()
    {
        const bool S = inputs[0];
        const bool R = inputs[1];

        if (S && R) {
            state.reset();
            std::cerr << "Warning: Invalid SR latch input: S and R are both true\n";
            return;
        }

        if (S) {
            state[0] = true;
            state[1] = false;
            return;
        }

        if (R) {
            state[0] = false;
            state[1] = true;
        }
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
