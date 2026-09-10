from typing import Union
from ._pylogic_core import LogicState, NativeWire, NativeANDGate, NativeOrGate, NativeXorGate, NativeNotGate

class Wire:
    """A high-level ergonomic digital wire with operator overloading, inspired by PyTorch"""

    def __init__(self, initial: Union[int, bool, LogicState, "Wire", None] = None, _native: NativeWire = None):
        if _native is not None:
            self._native = _native
            return

        if initial is None:
            self._native = NativeWire()
        elif isinstance(initial, Wire):
            self._native = NativeWire(initial._native.read())
        elif isinstance(initial, LogicState):
            self._native = NativeWire(initial)
        elif initial in (1, True, "1", "HIGH"):
            self._native = NativeWire(LogicState.HIGH)
        elif initial in (0, False, "0", "LOW"):
            self._native = NativeWire(LogicState.LOW)
        else:
            self._native = NativeWire(LogicState.UNKNOWN)

    @property
    def native(self) -> NativeWire:
        """Direct access to the underlying C++ logic::Wire."""
        return self._native

    @property
    def state(self) -> LogicState:
        """Get the 4-state LogicState (LOW, HIGH, UNKNOWN, HIGH_IMPEDANCE)."""
        return self._native.read()

    @state.setter
    def state(self, new_state: LogicState):
        self._native.write(new_state)

    @property
    def value(self) -> int:
        """Binary integer value (1 for HIGH, 0 otherwise)."""
        return 1 if self.state == LogicState.HIGH else 0

    @value.setter
    def value(self, val: Union[int, bool]):
        self._native.write(LogicState.HIGH if val else LogicState.LOW)

    @property
    def is_high(self) -> bool:
        return self.state == LogicState.HIGH

    @property
    def is_low(self) -> bool:
        return self.state == LogicState.LOW

    def toggle(self):
        """Invert the current state (LOW <-> HIGH)."""
        self.value = 0 if self.is_high else 1

    # PyTorch-like Operator Overloading
    def __and__(self, other: Union["Wire", int, bool]) -> "Wire":
        if not isinstance(other, Wire):
            other = Wire(other)
        out = Wire()
        gate = NativeANDGate(self._native, other._native, out._native)
        gate.evaluate()
        return out

    def __or__(self, other: Union["Wire", int, bool]) -> "Wire":
        if not isinstance(other, Wire):
            other = Wire(other)
        out = Wire()
        gate = NativeOrGate(self._native, other._native, out._native)
        gate.evaluate()
        return out

    def __xor__(self, other: Union["Wire", int, bool]) -> "Wire":
        if not isinstance(other, Wire):
            other = Wire(other)
        out = Wire()
        gate = NativeXorGate(self._native, other._native, out._native)
        gate.evaluate()
        return out

    def __invert__(self) -> "Wire":
        out = Wire()
        gate = NativeNotGate(self._native, out._native)
        gate.evaluate()
        return out

    def __int__(self) -> int:
        return self.value

    def __bool__(self) -> bool:
        return self.is_high

    def __repr__(self) -> str:
        state_str = str(self.state).split(".")[-1]
        return f"<Wire value={self.value} ({state_str})>"
