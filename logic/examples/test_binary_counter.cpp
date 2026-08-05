#include <iostream>
#include <cassert>
#include <vector>

#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include "signals/logicState.hpp"
#include "sequential/counters/BinaryCounter.hpp"

using logic::Wire;
using logic::Bus;
using logic::Clock;
using logic::LogicState;
using logic::BinaryCounter;

int read_bus_as_int(const Bus<4>& bus) {
    int val = 0;
    for (std::size_t i = 0; i < 4; ++i) {
        if (bus[i].read() == LogicState::HIGH) {
            val |= (1 << i);
        }
    }
    return val;
}

void test_binary_counter_4bit() {
    std::cout << "--- Testing 4-bit BinaryCounter ---\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Bus<4> count;

    BinaryCounter<4> counter(clock, enable, reset, count);

    auto step_clock = [&]() {
        counter.evaluate();
        clock.tick(); // LOW -> HIGH (rising edge)
        counter.evaluate();
        clock.tick(); // HIGH -> LOW
        counter.evaluate();
    };

    // 1. Initial evaluate
    counter.evaluate();
    std::cout << "Initial count: " << read_bus_as_int(count) << "\n";

    // 2. Count up from 0 to 15
    for (int expected = 1; expected <= 15; ++expected) {
        step_clock();
        int actual = read_bus_as_int(count);
        std::cout << "Step " << expected << " count: " << actual << std::endl;
        assert(actual == (expected % 16));
    }

    // 3. Overflow test (15 -> 0)
    step_clock();
    int actual_overflow = read_bus_as_int(count);
    std::cout << "After overflow count: " << actual_overflow << "\n";
    assert(actual_overflow == 0);

    // 4. Disable test (hold count)
    enable.write(LogicState::LOW);
    step_clock();
    step_clock();
    int actual_disabled = read_bus_as_int(count);
    std::cout << "Count when disabled: " << actual_disabled << "\n";
    assert(actual_disabled == 0);

    // Re-enable and count 2 steps to 2
    enable.write(LogicState::HIGH);
    step_clock();
    step_clock();
    int count_after_reenable = read_bus_as_int(count);
    std::cout << "Count after re-enable 2 steps (expected 2): " << count_after_reenable << "\n";
    assert(count_after_reenable == 2);

    // 5. Synchronous Reset test
    reset.write(LogicState::HIGH);
    step_clock();
    int actual_reset = read_bus_as_int(count);
    std::cout << "Count after reset (expected 0): " << actual_reset << "\n";
    assert(actual_reset == 0);

    // Release reset
    reset.write(LogicState::LOW);
    step_clock();
    std::cout << "Count after releasing reset: " << read_bus_as_int(count) << "\n";
    assert(read_bus_as_int(count) == 1);

    std::cout << "4-bit BinaryCounter Test Passed!\n\n";
}

int main() {
    test_binary_counter_4bit();
    return 0;
}
