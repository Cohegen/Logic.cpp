#include <iostream>
#include <cassert>

#include <logic/signals/wire.hpp>
#include <logic/signals/bus.hpp>
#include <logic/signals/clock.hpp>
#include <logic/signals/logicState.hpp>
#include <logic/sequential/counters/UpDownCounter.hpp>

using logic::Wire;
using logic::Bus;
using logic::Clock;
using logic::LogicState;
using logic::UpDownCounter;

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
void step_clock(UpDownCounter<N>& counter, Clock& clock) {
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
    Wire direction{LogicState::HIGH};
    Bus<4> count;
    UpDownCounter<4> counter(clock, enable, reset, direction, count);

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 0);
}

void test_up_down_counting() {
    std::cout << "Testing up and down counting...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Wire direction{LogicState::HIGH}; // HIGH = UP
    Bus<4> count;
    UpDownCounter<4> counter(clock, enable, reset, direction, count);

    counter.evaluate();
    assert(read_bus_as_int(count) == 0);

    // Count UP: 0 -> 1 -> 2 -> 3
    for (int expected = 1; expected <= 3; ++expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == expected);
    }

    // Switch direction to DOWN (LOW = DOWN)
    direction.write(LogicState::LOW);

    // Count DOWN: 3 -> 2 -> 1 -> 0
    for (int expected = 2; expected >= 0; --expected) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == expected);
    }
}

void test_enable_low_holds_current_value() {
    std::cout << "Testing enable LOW holds current value...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Wire direction{LogicState::HIGH};
    Bus<4> count;
    UpDownCounter<4> counter(clock, enable, reset, direction, count);

    for (int i = 0; i < 3; ++i) {
        step_clock(counter, clock);
    }
    assert(read_bus_as_int(count) == 3);

    enable.write(LogicState::LOW);
    for (int i = 0; i < 3; ++i) {
        step_clock(counter, clock);
    }
    assert(read_bus_as_int(count) == 3);
}

int main() {
    test_reset_initializes_count_to_zero();
    test_up_down_counting();
    test_enable_low_holds_current_value();
    std::cout << "UpDownCounter tests passed.\n";
    return 0;
}
