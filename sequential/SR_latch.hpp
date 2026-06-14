#ifndef SR_LATCH_HPP
#define SR_LATCH_HPP

#include <iostream>

class SR_Latch
{
protected:
    bool S;
    bool R;
    bool Q;
    bool Q_bar;

public:
    SR_Latch(bool s = false, bool r = false)
        : S(s), R(r), Q(false), Q_bar(true)
    {
        update();
    }

    virtual ~SR_Latch() = default;

    virtual void setInputs(bool s, bool r)
    {
        S = s;
        R = r;
    }

    virtual void update()
    {
        if (S && R) {
            Q = false;
            Q_bar = false;
            std::cerr << "Warning: Invalid SR latch input: S and R are both true\n";
            return;
        }

        if (S) {
            Q = true;
            Q_bar = false;
            return;
        }

        if (R) {
            Q = false;
            Q_bar = true;
        }
    }

    bool getQ() const
    {
        return Q;
    }

    bool getQ_bar() const
    {
        return Q_bar;
    }
};

#endif
