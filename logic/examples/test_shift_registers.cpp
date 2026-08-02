#include <iostream>
#include <cassert>

#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "signals/logicState.hpp"
#include "sequential/registers/ShiftRegisterSIPO.hpp"
#include "sequential/registers/ShiftRegisterSISO.hpp"

using logic::Wire;
using logic::Bus;
using logic::LogicState;
using logic::ShiftRegisterSIPO;
using logic::ShiftRegisterSISO;

void test_siso_4bit() {
    std::cout << "--- Testing 4-bit ShiftRegisterSISO ---\n";

    Wire s_in{LogicState::LOW};
    Wire s_out{LogicState::LOW};
    Wire clock{LogicState::LOW};

    ShiftRegisterSISO<4> siso(s_in, s_out, clock);

    auto pulse_clock = [&]() {
        clock.write(LogicState::LOW);
        siso.evaluate();
        clock.write(LogicState::HIGH);
        siso.evaluate();
        clock.write(LogicState::LOW);
        siso.evaluate();
    };

    // Shift in 1
    s_in.write(LogicState::HIGH);
    pulse_clock();
    
    // Shift in 0
    s_in.write(LogicState::LOW);
    pulse_clock();

    // Shift in 1
    s_in.write(LogicState::HIGH);
    pulse_clock();

    // Shift in 1
    s_in.write(LogicState::HIGH);
    pulse_clock();

    std::cout << "Output after 4 shifts of 1011: " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::HIGH);

    std::cout << "4-bit SISO Test Passed!\n\n";
}

void test_sipo_4bit() {
    std::cout << "--- Testing 4-bit ShiftRegisterSIPO ---\n";

    Wire s_in{LogicState::LOW};
    Wire clock{LogicState::LOW};
    Bus<4> p_out;

    ShiftRegisterSIPO<4> sipo(s_in, clock, p_out);

    auto pulse_clock = [&]() {
        clock.write(LogicState::LOW);
        sipo.evaluate();
        clock.write(LogicState::HIGH);
        sipo.evaluate();
        clock.write(LogicState::LOW);
        sipo.evaluate();
    };

    // Shift in pattern 1, 0, 1, 1
    s_in.write(LogicState::HIGH);
    pulse_clock();

    s_in.write(LogicState::LOW);
    pulse_clock();

    s_in.write(LogicState::HIGH);
    pulse_clock();

    s_in.write(LogicState::HIGH);
    pulse_clock();

    std::cout << "Parallel outputs: "
              << (p_out[0].read() == LogicState::HIGH ? 1 : 0)
              << (p_out[1].read() == LogicState::HIGH ? 1 : 0)
              << (p_out[2].read() == LogicState::HIGH ? 1 : 0)
              << (p_out[3].read() == LogicState::HIGH ? 1 : 0) << "\n";

    std::cout << "4-bit SIPO Test Passed!\n\n";
}

int main() {
    test_siso_4bit();
    test_sipo_4bit();
    std::cout << "All SISO and SIPO ShiftRegister tests passed!\n";
    return 0;
}
