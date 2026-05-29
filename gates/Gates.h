#ifndef GATES_H
#define GATES_H

#include <initializer_list>
#include <vector>

class Gates
{
private:
    std::vector<bool> inputs;

public:
    Gates(bool input1, bool input2);
    Gates(bool input1);
    Gates(std::initializer_list<bool> in);
    explicit Gates(std::vector<bool> in);

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
