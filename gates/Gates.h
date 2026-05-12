#ifndef GATES_H
#define GATES_H

class Gates
{
private:
    bool a;
    bool b;

public:
    Gates(bool input1, bool input2);
    Gates(bool input1);

    bool AND() const;
    bool OR() const;
    bool NOT() const;
    bool BUFFER() const;
    bool XOR() const;
    bool NAND() const;
    bool NOR() const;
    bool XNOR() const;
};

#endif
