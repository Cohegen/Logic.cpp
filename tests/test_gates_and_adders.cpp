#include <cassert>
#include <iostream>
#include <logic/logic.hpp>

void test_basic_gates() {
    logic::Wire a, b, out;

    logic::ANDGate and_gate(a, b, out);
    a.write(logic::LogicState::HIGH);
    b.write(logic::LogicState::HIGH);
    and_gate.evaluate();
    assert(out.read() == logic::LogicState::HIGH);

    b.write(logic::LogicState::LOW);
    and_gate.evaluate();
    assert(out.read() == logic::LogicState::LOW);

    logic::ORGate or_gate(a, b, out);
    or_gate.evaluate();
    assert(out.read() == logic::LogicState::HIGH);

    logic::XORGate xor_gate(a, b, out);
    xor_gate.evaluate();
    assert(out.read() == logic::LogicState::HIGH);

    b.write(logic::LogicState::HIGH);
    xor_gate.evaluate();
    assert(out.read() == logic::LogicState::LOW);

    std::cout << "[PASS] Basic Gates Test\n";
}

void test_adders() {
    logic::Wire a, b, cin, sum, carry;

    logic::HalfAdder ha(a, b, sum, carry);
    a.write(logic::LogicState::HIGH);
    b.write(logic::LogicState::HIGH);
    ha.evaluate();
    assert(sum.read() == logic::LogicState::LOW);
    assert(carry.read() == logic::LogicState::HIGH);

    logic::FullAdder fa(a, b, cin, sum, carry);
    cin.write(logic::LogicState::HIGH);
    fa.evaluate();
    assert(sum.read() == logic::LogicState::HIGH);
    assert(carry.read() == logic::LogicState::HIGH);

    std::cout << "[PASS] Adders Test\n";
}

int main() {
    test_basic_gates();
    test_adders();
    std::cout << "All Gates & Adders tests passed successfully!\n";
    return 0;
}
