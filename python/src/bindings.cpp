#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "signals/logicState.hpp"
#include "signals/wire.hpp"
#include "signals/bus.hpp"
#include "signals/clock.hpp"
#include "gates/AND.hpp"
#include "gates/OR.hpp"
#include "gates/NOT.hpp"
#include "gates/XOR.hpp"
#include "gates/NAND.hpp"
#include "gates/NOR.hpp"
#include "gates/XNOR.hpp"
#include "gates/BUFFER.hpp"
#include "combinational/adders/HalfAdder.hpp"
#include "combinational/adders/FullAdder.hpp"
#include "combinational/adders/RippleCarryAdder.hpp"
#include "combinational/alu/ALU.hpp"

namespace py = pybind11;

template <std::size_t N>
void bind_bus(py::module_& m, const char* name) {
    py::class_<logic::Bus<N>>(m, name)
        .def(py::init<>())
        .def("size", &logic::Bus<N>::size)
        .def("empty", &logic::Bus<N>::empty)
        .def("read_value", &logic::Bus<N>::read_value)
        .def("write_value", &logic::Bus<N>::write_value)
        .def("write", &logic::Bus<N>::write)
        .def("clear", &logic::Bus<N>::clear)
        .def("at", [](logic::Bus<N>& self, std::size_t index) -> logic::Wire& {
            return self.at(index);
        }, py::return_value_policy::reference_internal)
        .def("__getitem__", [](logic::Bus<N>& self, std::size_t index) -> logic::Wire& {
            return self.at(index);
        }, py::return_value_policy::reference_internal);
}

PYBIND11_MODULE(_pylogic_core, m) {
    m.doc() = "C++ backend for pylogic digital hardware simulator";

    // 1. LogicState Enum
    py::enum_<logic::LogicState>(m, "LogicState")
        .value("LOW", logic::LogicState::LOW)
        .value("HIGH", logic::LogicState::HIGH)
        .value("UNKNOWN", logic::LogicState::UNKNOWN)
        .value("HIGH_IMPEDANCE", logic::LogicState::HIGH_IMPEDANCE)
        .export_values();

    // 2. Wire
    py::class_<logic::Wire>(m, "NativeWire")
        .def(py::init<>())
        .def(py::init<logic::LogicState>())
        .def("write", &logic::Wire::write)
        .def("read", &logic::Wire::read);

    // 3. Clock
    py::class_<logic::Clock>(m, "NativeClock")
        .def(py::init<>())
        .def("tick", &logic::Clock::tick)
        .def("reset", &logic::Clock::reset)
        .def("cycle", &logic::Clock::cycle)
        .def("state", &logic::Clock::state)
        .def("is_high", &logic::Clock::is_high)
        .def("is_low", &logic::Clock::is_low);

    // 4. Common Buses
    bind_bus<2>(m, "NativeBus2");
    bind_bus<3>(m, "NativeBus3");
    bind_bus<4>(m, "NativeBus4");
    bind_bus<8>(m, "NativeBus8");
    bind_bus<16>(m, "NativeBus16");
    bind_bus<32>(m, "NativeBus32");
    bind_bus<64>(m, "NativeBus64");

    // 5. Primitive Gates
    py::class_<logic::ANDGate>(m, "NativeANDGate")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>())
        .def("evaluate", &logic::ANDGate::evaluate);

    py::class_<logic::OrGate>(m, "NativeOrGate")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>())
        .def("evaluate", &logic::OrGate::evaluate);

    py::class_<logic::XorGate>(m, "NativeXorGate")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>())
        .def("evaluate", &logic::XorGate::evaluate);

    py::class_<logic::NotGate>(m, "NativeNotGate")
        .def(py::init<logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>())
        .def("evaluate", &logic::NotGate::evaluate);

    py::class_<logic::NandGate>(m, "NativeNandGate")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>())
        .def("evaluate", &logic::NandGate::evaluate);

    py::class_<logic::NorGate>(m, "NativeNorGate")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>())
        .def("evaluate", &logic::NorGate::evaluate);

    py::class_<logic::XnorGate>(m, "NativeXnorGate")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>())
        .def("evaluate", &logic::XnorGate::evaluate);

    py::class_<logic::BufferGate>(m, "NativeBufferGate")
        .def(py::init<logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>())
        .def("evaluate", &logic::BufferGate::evaluate);

    // 6. Adders
    py::class_<logic::HalfAdder>(m, "NativeHalfAdder")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>(), py::keep_alive<1, 5>())
        .def("evaluate", &logic::HalfAdder::evaluate);

    py::class_<logic::FullAdder>(m, "NativeFullAdder")
        .def(py::init<logic::Wire&, logic::Wire&, logic::Wire&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>(), py::keep_alive<1, 5>(), py::keep_alive<1, 6>())
        .def("evaluate", &logic::FullAdder::evaluate);

    // 7. RippleCarryAdder<32>
    py::class_<logic::RippleCarryAdder<32>>(m, "NativeRippleCarryAdder32")
        .def(py::init<logic::Bus<32>&, logic::Bus<32>&, logic::Wire&, logic::Bus<32>&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>(), py::keep_alive<1, 5>(), py::keep_alive<1, 6>())
        .def("evaluate", &logic::RippleCarryAdder<32>::evaluate);

    // 8. ALU<32>
    py::class_<logic::ALU<32>>(m, "NativeALU32")
        .def(py::init<logic::Bus<32>&, logic::Bus<32>&, logic::Bus<3>&, logic::Bus<32>&, logic::Wire&, logic::Wire&>(),
             py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::keep_alive<1, 4>(), py::keep_alive<1, 5>(), py::keep_alive<1, 6>(), py::keep_alive<1, 7>())
        .def("evaluate", &logic::ALU<32>::evaluate);
}
