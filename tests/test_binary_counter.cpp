#include <iostream>
#include <cassert>

#include <logic/signals/wire.hpp>
#include <logic/signals/bus.hpp>
#include <logic/signals/clock.hpp>
#include <logic/signals/logicState.hpp>
#include <logic/sequential/counters/BinaryCounter.hpp>

using logic::Wire;
using logic::Bus;
using logic::Clock;
using logic::LogicState;
using logic::BinaryCounter;

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
void step_clock(BinaryCounter<N>& counter, Clock& clock) {
    counter.evaluate();
    clock.tick(); // LOW -> HIGH
    counter.evaluate();
    clock.tick(); // HIGH -> LOW
    counter.evaluate();
}

void test_reset_initializes_count_to_zero() {
    std::cout << "Testing reset initializes count to zero...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::HIGH};
    Bus<4> count;
    BinaryCounter<4> counter(clock, enable, reset, count);

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 0);
}

void test_counter_increments_over_several_cycles() {
    std::cout << "Testing counter increments over several cycles...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Bus<4> count;
    BinaryCounter<4> counter(clock, enable, reset, count);

    counter.evaluate();
    assert(read_bus_as_int(count) == 0);

    for (int expected = 1; expected <= 6; ++expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == expected);
    }
}

void test_enable_low_holds_current_value() {
    std::cout << "Testing enable LOW holds current value...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Bus<4> count;
    BinaryCounter<4> counter(clock, enable, reset, count);

    for (int i = 0; i < 5; ++i) {
        step_clock(counter, clock);
    }
    assert(read_bus_as_int(count) == 5);

    enable.write(LogicState::LOW);
    for (int i = 0; i < 3; ++i) {
        step_clock(counter, clock);
    }
    assert(read_bus_as_int(count) == 5);
}

void test_4bit_counter_wraps_from_15_to_0() {
    std::cout << "Testing 4-bit counter wraps from 15 to 0...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Bus<4> count;
    BinaryCounter<4> counter(clock, enable, reset, count);

    for (int expected = 1; expected <= 15; ++expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == expected);
    }

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 0);
}

int main() {
    test_reset_initializes_count_to_zero();
    test_counter_increments_over_several_cycles();
    test_enable_low_holds_current_value();
    test_4bit_counter_wraps_from_15_to_0();
    std::cout << "BinaryCounter tests passed.\n";
    return 0;
}
