#include <iostream>
#include "Gates.h"
#include "halfAdder.hpp"
#include "SR_latch.hpp"

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

    SR_Latch latch;
    std::cout << "\nSR latch initial state\n";
    std::cout << "Q: " << latch.getQ() << ", Q_bar: " << latch.getQ_bar() << '\n';

    latch.setInputs(true, false);
    latch.update();
    std::cout << "Set S=true, R=false -> Q: " << latch.getQ()
              << ", Q_bar: " << latch.getQ_bar() << '\n';

    latch.setInputs(false, false);
    latch.update();
    std::cout << "Hold S=false, R=false -> Q: " << latch.getQ()
              << ", Q_bar: " << latch.getQ_bar() << '\n';

    latch.setInputs(false, true);
    latch.update();
    std::cout << "Reset S=false, R=true -> Q: " << latch.getQ()
              << ", Q_bar: " << latch.getQ_bar() << '\n';

    return 0;
}
