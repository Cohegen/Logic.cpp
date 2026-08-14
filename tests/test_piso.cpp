#include <iostream>
#include <cassert>
#include <vector>

#include <logic/signals/wire.hpp>
#include <logic/signals/bus.hpp>
#include <logic/signals/logicState.hpp>
#include <logic/sequential/registers/ShiftRegisterPISO.hpp>

using logic::Wire;
using logic::Bus;
using logic::LogicState;
using logic::ShiftRegisterPISO;

void test_piso_4bit() {
    std::cout << "--- Testing 4-bit ShiftRegisterPISO ---\n";

    Bus<4> p_in;
    Wire s_in{LogicState::LOW};
    Wire s_out{LogicState::LOW};
    Wire clock{LogicState::LOW};
    Wire load{LogicState::LOW};

    ShiftRegisterPISO<4> piso(p_in, s_in, s_out, clock, load);

    // Helper to pulse clock LOW -> HIGH -> LOW
    auto pulse_clock = [&]() {
        clock.write(LogicState::LOW);
        piso.evaluate();
        clock.write(LogicState::HIGH);
        piso.evaluate();
        clock.write(LogicState::LOW);
        piso.evaluate();
    };

    // Set parallel input to 1011 (p_in[0]=1, p_in[1]=0, p_in[2]=1, p_in[3]=1)
    p_in[0].write(LogicState::HIGH);
    p_in[1].write(LogicState::LOW);
    p_in[2].write(LogicState::HIGH);
    p_in[3].write(LogicState::HIGH);

    // Enable Parallel Load
    load.write(LogicState::HIGH);
    piso.evaluate();

    std::cout << "Loading Parallel Input 1011 into 4-bit PISO register...\n";
    pulse_clock();

    // After loading, serial_out should present the LSB or MSB depending on setup (stage N-1, i.e., bit 3 = 1)
    std::cout << "Serial output after load: " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::HIGH);

    // Switch to Shift mode (load = 0)
    load.write(LogicState::LOW);
    s_in.write(LogicState::LOW); // Fill incoming serial stream with 0s
    piso.evaluate();

    // Clock pulse 1 (Shift 1)
    pulse_clock();
    std::cout << "Shift 1 output: " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::HIGH); // bit 2 value

    // Clock pulse 2 (Shift 2)
    pulse_clock();
    std::cout << "Shift 2 output: " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::LOW);  // bit 1 value

    // Clock pulse 3 (Shift 3)
    pulse_clock();
    std::cout << "Shift 3 output: " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::HIGH); // bit 0 value

    // Clock pulse 4 (Shift 4 - serial_in pushed through)
    pulse_clock();
    std::cout << "Shift 4 output (serial_in): " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::LOW);

    std::cout << "4-bit PISO Test Passed!\n\n";
}

void test_piso_1bit() {
    std::cout << "--- Testing 1-bit ShiftRegisterPISO ---\n";

    Bus<1> p_in;
    Wire s_in{LogicState::LOW};
    Wire s_out{LogicState::LOW};
    Wire clock{LogicState::LOW};
    Wire load{LogicState::LOW};

    ShiftRegisterPISO<1> piso(p_in, s_in, s_out, clock, load);

    auto pulse_clock = [&]() {
        clock.write(LogicState::LOW);
        piso.evaluate();
        clock.write(LogicState::HIGH);
        piso.evaluate();
        clock.write(LogicState::LOW);
        piso.evaluate();
    };

    p_in[0].write(LogicState::HIGH);
    load.write(LogicState::HIGH);
    pulse_clock();

    std::cout << "Loaded 1 into 1-bit register. Output: " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::HIGH);

    load.write(LogicState::LOW);
    s_in.write(LogicState::LOW);
    pulse_clock();

    std::cout << "Shifted 0 from s_in. Output: " << (s_out.read() == LogicState::HIGH ? 1 : 0) << "\n";
    assert(s_out.read() == LogicState::LOW);

    std::cout << "1-bit PISO Test Passed!\n\n";
}

int main() {
    test_piso_1bit();
    test_piso_4bit();
    std::cout << "All ShiftRegisterPISO tests completed successfully!\n";
    return 0;
}
