from typing import Optional, Tuple
from ._pylogic_core import (
    NativeANDGate,
    NativeOrGate,
    NativeXorGate,
    NativeNotGate,
    NativeNandGate,
    NativeNorGate,
    NativeXnorGate,
    NativeBufferGate,
    NativeHalfAdder,
    NativeFullAdder,
    LogicState,
)
from .wire import Wire

class BaseGate:
    def __init__(self, native_gate, *outputs: Wire):
        self._native = native_gate
        self._outputs = outputs

    def evaluate(self):
        self._native.evaluate()
        return self._outputs[0] if len(self._outputs) == 1 else self._outputs

    def __call__(self):
        return self.evaluate()


def AND(a: Wire, b: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeANDGate(a.native, b.native, out_wire.native)
    return BaseGate(gate, out_wire)

def OR(a: Wire, b: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeOrGate(a.native, b.native, out_wire.native)
    return BaseGate(gate, out_wire)

def XOR(a: Wire, b: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeXorGate(a.native, b.native, out_wire.native)
    return BaseGate(gate, out_wire)

def NOT(a: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeNotGate(a.native, out_wire.native)
    return BaseGate(gate, out_wire)

def NAND(a: Wire, b: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeNandGate(a.native, b.native, out_wire.native)
    return BaseGate(gate, out_wire)

def NOR(a: Wire, b: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeNorGate(a.native, b.native, out_wire.native)
    return BaseGate(gate, out_wire)

def XNOR(a: Wire, b: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeXnorGate(a.native, b.native, out_wire.native)
    return BaseGate(gate, out_wire)

def Buffer(a: Wire, out: Optional[Wire] = None) -> BaseGate:
    out_wire = out if out is not None else Wire()
    gate = NativeBufferGate(a.native, out_wire.native)
    return BaseGate(gate, out_wire)

class HalfAdder:
    def __init__(self, a: Wire, b: Wire, sum_wire: Optional[Wire] = None, carry_wire: Optional[Wire] = None):
        self.a = a
        self.b = b
        self.sum = sum_wire if sum_wire is not None else Wire()
        self.carry = carry_wire if carry_wire is not None else Wire()
        self._native = NativeHalfAdder(self.a.native, self.b.native, self.sum.native, self.carry.native)

    def evaluate(self) -> Tuple[Wire, Wire]:
        self._native.evaluate()
        return self.sum, self.carry

    def __call__(self) -> Tuple[Wire, Wire]:
        return self.evaluate()


class FullAdder:
    def __init__(self, a: Wire, b: Wire, cin: Optional[Wire] = None, sum_wire: Optional[Wire] = None, carry_wire: Optional[Wire] = None):
        self.a = a
        self.b = b
        self.cin = cin if cin is not None else Wire(0)
        self.sum = sum_wire if sum_wire is not None else Wire()
        self.carry = carry_wire if carry_wire is not None else Wire()
        self._native = NativeFullAdder(self.a.native, self.b.native, self.cin.native, self.sum.native, self.carry.native)

    def evaluate(self) -> Tuple[Wire, Wire]:
        self._native.evaluate()
        return self.sum, self.carry

    def __call__(self) -> Tuple[Wire, Wire]:
        return self.evaluate()
