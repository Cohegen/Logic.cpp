#pragma once

#include <cstddef>
#include <initializer_list>

class Gates {
public:
    static constexpr std::size_t maxInputs = 64;

    Gates(bool input1, bool input2);
    Gates(bool input1);
    explicit Gates(std::initializer_list<bool> in);

    [[nodiscard]] bool AND() const;
    [[nodiscard]] bool OR() const;
    [[nodiscard]] bool NOT() const;
    [[nodiscard]] bool BUFFER() const;
    [[nodiscard]] bool XOR() const;
    [[nodiscard]] bool NAND() const;
    [[nodiscard]] bool NOR() const;
    [[nodiscard]] bool XNOR() const;

private:
    bool inputs[maxInputs]{};
    std::size_t inputCount{};
};
