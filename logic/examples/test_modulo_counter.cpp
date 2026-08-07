#include <iostream>
#include <cassert>

#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include "signals/logicState.hpp"
#include "sequential/counters/ModuloCounter.hpp"

using logic::Wire;
using logic::Bus;
using logic::Clock;
using logic::LogicState;
using logic::ModuloCounter;

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
void set_bus_from_int(Bus<N>& bus, int val) {
    for (std::size_t i = 0; i < N; ++i) {
        if ((val >> i) & 1) {
            bus[i].write(LogicState::HIGH);
        } else {
            bus[i].write(LogicState::LOW);
        }
    }
}

template <std::size_t N>
void step_clock(ModuloCounter<N>& counter, Clock& clock) {
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
    Bus<4> modulo;
    set_bus_from_int(modulo, 5);
    Bus<4> count;
    ModuloCounter<4> counter(clock, enable, reset, modulo, count);

    step_clock(counter, clock);
    assert(read_bus_as_int(count) == 0);
}

void test_modulo_5_counting_cycle() {
    std::cout << "Testing Modulo-5 counting cycle (0 -> 1 -> 2 -> 3 -> 4 -> 0)...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Bus<4> modulo;
    set_bus_from_int(modulo, 5); // Modulo 5 (counts 0, 1, 2, 3, 4 then wraps to 0)
    Bus<4> count;
    ModuloCounter<4> counter(clock, enable, reset, modulo, count);

    counter.evaluate();
    assert(read_bus_as_int(count) == 0);

    // Sequence expected: 1, 2, 3, 4, 0, 1, 2
    int expected_sequence[] = {1, 2, 3, 4, 0, 1, 2};
    for (int expected : expected_sequence) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == expected);
    }
}

void test_enable_low_holds_current_value() {
    std::cout << "Testing enable LOW holds current value...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Bus<4> modulo;
    set_bus_from_int(modulo, 5);
    Bus<4> count;
    ModuloCounter<4> counter(clock, enable, reset, modulo, count);

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

void test_modulo_3_counting_cycle() {
    std::cout << "Testing Modulo-3 counting cycle (0 -> 1 -> 2 -> 0)...\n";

    Clock clock;
    Wire enable{LogicState::HIGH};
    Wire reset{LogicState::LOW};
    Bus<4> modulo;
    set_bus_from_int(modulo, 3); // Modulo 3
    Bus<4> count;
    ModuloCounter<4> counter(clock, enable, reset, modulo, count);

    counter.evaluate();
    assert(read_bus_as_int(count) == 0);

    int expected_sequence[] = {1, 2, 0, 1, 2, 0};
    for (int expected : expected_sequence) {
        step_clock(counter, clock);
        assert(read_bus_as_int(count) == expected);
    }
}

int main() {
    test_reset_initializes_count_to_zero();
    test_modulo_5_counting_cycle();
    test_enable_low_holds_current_value();
    test_modulo_3_counting_cycle();
    std::cout << "ModuloCounter tests passed successfully.\n";
    return 0;
}
