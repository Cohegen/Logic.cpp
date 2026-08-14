#include <iostream>
#include <cassert>
#include <vector>

#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include "signals/logicState.hpp"
#include "sequential/counters/JohnsonCounter.hpp"

using logic::Wire;
using logic::Bus;
using logic::Clock;
using logic::LogicState;
using logic::JohnsonCounter;

template <std::size_t N>
int read_bus_as_int(const Bus<N>& bus) {
    int val = 0;
    for (std::size_t i = 0; i < N; ++i) {
        if (bus[i].read() == LogicState::HIGH) {
            val |= (1 << i);
        }
    }
    return val;
}

template <std::size_t N>
void step_clock(JohnsonCounter<N>& counter, Clock& clock) {
    counter.evaluate();
    clock.tick(); // LOW -> HIGH
    counter.evaluate();
    clock.tick(); // HIGH -> LOW
    counter.evaluate();
}

void test_reset_initializes_count_to_zero() {
    std::cout << "Testing JohnsonCounter reset initializes count to zero...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<4> count;
    JohnsonCounter<4> counter(clock, enable, reset, count);

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 0);
}

void test_4bit_johnson_counter_sequence() {
    std::cout << "Testing 4-bit JohnsonCounter full 8-state sequence...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<4> count;
    JohnsonCounter<4> counter(clock, enable, reset, count);

    // Synchronous reset
    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 0);

    // Disable reset and count
    reset.write(LogicState::LOW);

    // Expected sequence for 4-bit Johnson counter (LSB at index 0):
    // 0: 0000 (0)
    // 1: 0001 (1)
    // 2: 0011 (3)
    // 3: 0111 (7)
    // 4: 1111 (15)
    // 5: 1110 (14)
    // 6: 1100 (12)
    // 7: 1000 (8)
    // 8: 0000 (0) - wrap around
    std::vector<int> expected = {1, 3, 7, 15, 14, 12, 8, 0};

    for (int exp : expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == exp);
    }
}

void test_enable_low_holds_current_value() {
    std::cout << "Testing JohnsonCounter enable LOW holds current value...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<4> count;
    JohnsonCounter<4> counter(clock, enable, reset, count);

    step_clock(counter, clock);
    reset.write(LogicState::LOW);

    // Step 3 cycles: 0000 -> 0001 -> 0011 -> 0111 (7)
    step_clock(counter, clock);
    step_clock(counter, clock);
    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 7);

    // Disable counting
    enable.write(LogicState::LOW);
    for (int i = 0; i < 3; ++i) {
        step_clock(counter, clock);
    }
    assert(read_bus_as_int(count) == 7);
}

void test_3bit_johnson_counter_sequence() {
    std::cout << "Testing 3-bit JohnsonCounter sequence...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<3> count;
    JohnsonCounter<3> counter(clock, enable, reset, count);

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 0);

    reset.write(LogicState::LOW);

    // Expected for 3-bit: 000 (0) -> 001 (1) -> 011 (3) -> 111 (7) -> 110 (6) -> 100 (4) -> 000 (0)
    std::vector<int> expected = {1, 3, 7, 6, 4, 0};

    for (int exp : expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == exp);
    }
}

int main() {
    test_reset_initializes_count_to_zero();
    test_4bit_johnson_counter_sequence();
    test_enable_low_holds_current_value();
    test_3bit_johnson_counter_sequence();
    std::cout << "All JohnsonCounter tests passed successfully!\n";
    return 0;
}
