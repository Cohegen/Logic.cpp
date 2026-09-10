import os
import sys
import unittest

# Ensure pylogic package is on path
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
sys.path.insert(0, parent_dir)

import pylogic
from pylogic import Wire, Bus, Clock, LogicState, AND, OR, XOR, NOT, HalfAdder, FullAdder

class TestPyLogic(unittest.TestCase):
    def test_wire_initialization_and_properties(self):
        w1 = Wire(1)
        self.assertEqual(w1.value, 1)
        self.assertTrue(w1.is_high)
        self.assertFalse(w1.is_low)
        self.assertEqual(w1.state, LogicState.HIGH)

        w0 = Wire(0)
        self.assertEqual(w0.value, 0)
        self.assertFalse(w0.is_high)
        self.assertTrue(w0.is_low)
        self.assertEqual(w0.state, LogicState.LOW)

        w0.toggle()
        self.assertEqual(w0.value, 1)

    def test_wire_operators(self):
        a = Wire(1)
        b = Wire(0)

        # PyTorch-like operator overloading
        and_res = a & b
        self.assertEqual(and_res.value, 0)

        or_res = a | b
        self.assertEqual(or_res.value, 1)

        xor_res = a ^ b
        self.assertEqual(xor_res.value, 1)

        not_res = ~a
        self.assertEqual(not_res.value, 0)

    def test_bus_values_and_slicing(self):
        bus = Bus(32, value=0xAABBCCDD)
        self.assertEqual(bus.value, 0xAABBCCDD)
        self.assertEqual(len(bus), 32)

        # Slicing lowest byte: 0xDD = 221
        byte0 = bus[0:8]
        self.assertEqual(byte0.value, 0xDD)

        # Slicing second byte: 0xCC = 204
        byte1 = bus[8:16]
        self.assertEqual(byte1.value, 0xCC)

        # Bit indexing
        wire0 = bus[0]
        self.assertEqual(wire0.value, 1) # 0xDD ends in 1

    def test_bus_addition(self):
        bus_a = Bus(32, value=100)
        bus_b = Bus(32, value=250)
        sum_bus = bus_a + bus_b
        self.assertEqual(sum_bus.value, 350)

    def test_half_adder(self):
        a = Wire(1)
        b = Wire(1)
        adder = HalfAdder(a, b)
        sum_wire, carry_wire = adder()
        self.assertEqual(sum_wire.value, 0)
        self.assertEqual(carry_wire.value, 1)

    def test_full_adder(self):
        a = Wire(1)
        b = Wire(1)
        cin = Wire(1)
        adder = FullAdder(a, b, cin)
        sum_wire, carry_wire = adder()
        self.assertEqual(sum_wire.value, 1) # 1+1+1 = 3 -> sum=1, carry=1
        self.assertEqual(carry_wire.value, 1)

    def test_clock(self):
        clk = Clock()
        self.assertEqual(clk.cycle(), 0)
        clk.tick()
        self.assertEqual(clk.cycle(), 1)
        self.assertTrue(clk.is_high())

if __name__ == "__main__":
    print("=" * 60)
    print("      RUNNING PYLOGIC TEST SUITE")
    print("=" * 60)
    unittest.main(verbosity=2)
