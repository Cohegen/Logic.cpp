#include <iostream>
#include <cassert>
#include <vector>

#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include "signals/logicState.hpp"
#include "sequential/counters/RingCounter.hpp"

using logic::Wire;
using logic::Bus;
using logic::Clock;
using logic::LogicState;
using logic::RingCounter;

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
void step_clock(RingCounter<N>& counter, Clock& clock) {
    counter.evaluate();
    clock.tick(); // LOW -> HIGH
    counter.evaluate();
    clock.tick(); // HIGH -> LOW
    counter.evaluate();
}

void test_reset_initializes_count_to_one_hot() {
    std::cout << "Testing RingCounter reset initializes count to 0b0001...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<4> count;
    RingCounter<4> counter(clock, enable, reset, count);

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 1);
}

void test_4bit_ring_counter_rotation() {
    std::cout << "Testing 4-bit RingCounter rotation sequence...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<4> count;
    RingCounter<4> counter(clock, enable, reset, count);

    // Synchronous reset sets initial state 0001 (1)
    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 1);

    reset.write(LogicState::LOW);

    // Expected 4-state sequence: 0001 (1) -> 0010 (2) -> 0100 (4) -> 1000 (8) -> 0001 (1)
    std::vector<int> expected = {2, 4, 8, 1};

    for (int exp : expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == exp);
    }
}

void test_enable_low_holds_current_value() {
    std::cout << "Testing RingCounter enable LOW holds current value...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<4> count;
    RingCounter<4> counter(clock, enable, reset, count);

    step_clock(counter, clock);
    reset.write(LogicState::LOW);

    // Step 2 cycles: 0001 -> 0010 -> 0100 (4)
    step_clock(counter, clock);
    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 4);

    // Disable counting
    enable.write(LogicState::LOW);
    for (int i = 0; i < 3; ++i) {
        step_clock(counter, clock);
    }
    assert(read_bus_as_int(count) == 4);
}

void test_5bit_ring_counter_rotation() {
    std::cout << "Testing 5-bit RingCounter rotation...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<5> count;
    RingCounter<5> counter(clock, enable, reset, count);

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 1);

    reset.write(LogicState::LOW);

    std::vector<int> expected = {2, 4, 8, 16, 1};

    for (int exp : expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == exp);
    }
}

int main() {
    test_reset_initializes_count_to_one_hot();
    test_4bit_ring_counter_rotation();
    test_enable_low_holds_current_value();
    test_5bit_ring_counter_rotation();
    std::cout << "All RingCounter tests passed successfully!\n";
    return 0;
}
