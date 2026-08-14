#include <cassert>
#include <iostream>
#include <logic/logic.hpp>

template <std::size_t N>
void set_bus_val(logic::Bus<N>& bus, unsigned int val) {
    for (std::size_t i = 0; i < N; ++i) {
        bus[i].write((val & (1u << i)) ? logic::LogicState::HIGH : logic::LogicState::LOW);
    }
}

template <std::size_t N>
unsigned int read_bus_val(const logic::Bus<N>& bus) {
    unsigned int val = 0;
    for (std::size_t i = 0; i < N; ++i) {
        if (bus[i].read() == logic::LogicState::HIGH) {
            val |= (1u << i);
        }
    }
    return val;
}

void test_alu_operations() {
    constexpr std::size_t BITS = 4;
    logic::Bus<BITS> a, b, result;
    logic::Bus<3> opcode;
    logic::Wire zero, carry;

    logic::ALU<BITS> alu(a, b, opcode, result, zero, carry);

    // Opcode 0: ADD (5 + 3 = 8)
    set_bus_val(a, 5);
    set_bus_val(b, 3);
    set_bus_val(opcode, 0);
    alu.evaluate();
    assert(read_bus_val(result) == 8);
    assert(zero.read() == logic::LogicState::LOW);

    // Opcode 1: SUB (5 - 5 = 0)
    set_bus_val(b, 5);
    set_bus_val(opcode, 1);
    alu.evaluate();
    assert(read_bus_val(result) == 0);
    assert(zero.read() == logic::LogicState::HIGH);

    // Opcode 2: AND (0b1100 & 0b1010 = 0b1000)
    set_bus_val(a, 0b1100);
    set_bus_val(b, 0b1010);
    set_bus_val(opcode, 2);
    alu.evaluate();
    assert(read_bus_val(result) == 0b1000);

    // Opcode 3: OR (0b1100 | 0b1010 = 0b1110)
    set_bus_val(opcode, 3);
    alu.evaluate();
    assert(read_bus_val(result) == 0b1110);

    // Opcode 4: XOR (0b1100 ^ 0b1010 = 0b0110)
    set_bus_val(opcode, 4);
    alu.evaluate();
    assert(read_bus_val(result) == 0b0110);

    std::cout << "[PASS] ALU Operations Test\n";
}

int main() {
    test_alu_operations();
    std::cout << "ALU unit tests passed successfully!\n";
    return 0;
}
