#include <iostream>
#include "Gates.h"
#include "halfAdder.hpp"

int main()
{
    const Gates twoInputGate(true, false);
    const Gates oneInputGate(true);

    std::cout << std::boolalpha;
    std::cout << "A = true, B = false\n";
    std::cout << "AND: " << twoInputGate.AND() << '\n';
    std::cout << "OR: " << twoInputGate.OR() << '\n';
    std::cout << "XOR: " << twoInputGate.XOR() << '\n';
    std::cout << "NAND: " << twoInputGate.NAND() << '\n';
    std::cout << "NOR: " << twoInputGate.NOR() << '\n';
    std::cout << "XNOR: " << twoInputGate.XNOR() << '\n';
    std::cout << "NOT A: " << oneInputGate.NOT() << '\n';
    std::cout << "BUFFER A: " << oneInputGate.BUFFER() << '\n';

    const HalfAdder halfAdder(true, true);
    std::cout << "\nHalf adder: A = true, B = true\n";
    std::cout << "SUM: " << halfAdder.sum() << '\n';
    std::cout << "CARRY: " << halfAdder.carry() << '\n';

    return 0;
}
