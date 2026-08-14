#include <iostream>

#include "Gates.h"
#include "combinational/adders/HalfAdder.hpp"
#include "sequential/latches/SRLatch.hpp"
#include "signals/logicState.hpp"
#include "signals/wire.hpp"

using logic::HalfAdder;
using logic::LogicState;
using logic::Wire;

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

    Wire a{LogicState::HIGH};
    Wire b{LogicState::HIGH};
    Wire sum;
    Wire carry;

    HalfAdder halfAdder(a, b, sum, carry);
    halfAdder.evaluate();
    std::cout << "\nHalf adder: A = true, B = true\n";
    std::cout << "SUM: " << sum.read() << '\n';
    std::cout << "CARRY: " << carry.read() << '\n';

    Wire s_wire{LogicState::LOW};
    Wire r_wire{LogicState::LOW};
    Wire q_wire;
    Wire q_bar_wire;
    logic::SRLatch latch(s_wire, r_wire, q_wire, q_bar_wire);
    latch.evaluate();
    std::cout << "\nSR latch initial state\n";
    std::cout << "Q: " << q_wire.read() << ", Q_bar: " << q_bar_wire.read() << '\n';

    s_wire.write(LogicState::HIGH);
    r_wire.write(LogicState::LOW);
    latch.evaluate();
    std::cout << "Set S=true, R=false -> Q: " << q_wire.read()
              << ", Q_bar: " << q_bar_wire.read() << '\n';

    s_wire.write(LogicState::LOW);
    r_wire.write(LogicState::LOW);
    latch.evaluate();
    std::cout << "Hold S=false, R=false -> Q: " << q_wire.read()
              << ", Q_bar: " << q_bar_wire.read() << '\n';

    s_wire.write(LogicState::LOW);
    r_wire.write(LogicState::HIGH);
    latch.evaluate();
    std::cout << "Reset S=false, R=true -> Q: " << q_wire.read()
              << ", Q_bar: " << q_bar_wire.read() << '\n';

    return 0;
}
