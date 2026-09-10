from typing import Union, List
from ._pylogic_core import (
    NativeBus2,
    NativeBus3,
    NativeBus4,
    NativeBus8,
    NativeBus16,
    NativeBus32,
    NativeBus64,
    NativeRippleCarryAdder32,
    NativeWire,
    LogicState,
)
from .wire import Wire

_NATIVE_BUS_MAP = {
    2: NativeBus2,
    3: NativeBus3,
    4: NativeBus4,
    8: NativeBus8,
    16: NativeBus16,
    32: NativeBus32,
    64: NativeBus64,
}

class Bus:
    """A high-level ergonomic hardware Bus wrapping templated C++ logic::Bus<N>."""

    def __init__(self, width: int = 32, value: int = 0, _native = None):
        self._width = width
        if _native is not None:
            self._native = _native
            return

        if width in _NATIVE_BUS_MAP:
            self._native = _NATIVE_BUS_MAP[width]()
        else:
            # Fallback to nearest larger standard width or closest native
            if width <= 8:
                self._native = NativeBus8()
            elif width <= 16:
                self._native = NativeBus16()
            elif width <= 32:
                self._native = NativeBus32()
            else:
                self._native = NativeBus64()

        if value != 0:
            self.value = value

    @property
    def native(self):
        """Underlying C++ logic::Bus<N> instance."""
        return self._native

    @property
    def width(self) -> int:
        return self._width

    @property
    def value(self) -> int:
        """Numeric unsigned value carried across the bus."""
        mask = (1 << self._width) - 1
        return self._native.read_value() & mask

    @value.setter
    def value(self, val: int):
        mask = (1 << self._width) - 1
        self._native.write_value(int(val) & mask)

    def clear(self):
        self._native.clear()

    def __len__(self) -> int:
        return self._width

    def __getitem__(self, item: Union[int, slice]):
        if isinstance(item, int):
            if item < 0:
                item += self._width
            if item < 0 or item >= self._width:
                raise IndexError(f"Bus index {item} out of range [0, {self._width - 1}]")
            # Return Wire wrapping the native reference
            native_wire = self._native.at(item)
            return Wire(_native=native_wire)
        elif isinstance(item, slice):
            start = item.start if item.start is not None else 0
            stop = item.stop if item.stop is not None else self._width
            slice_width = stop - start
            if slice_width <= 0:
                raise ValueError(f"Invalid slice width: {slice_width}")
            # Extract sliced bits
            val = (self.value >> start) & ((1 << slice_width) - 1)
            return Bus(width=slice_width, value=val)
        else:
            raise TypeError(f"Invalid bus index type: {type(item)}")

    def __setitem__(self, index: int, wire_or_val: Union[Wire, int]):
        if isinstance(wire_or_val, Wire):
            st = wire_or_val.state
        else:
            st = LogicState.HIGH if wire_or_val else LogicState.LOW
        self._native.at(index).write(st)

    # Arithmetic & Logic Operators
    def __add__(self, other: Union["Bus", int]) -> "Bus":
        if self._width == 32 and isinstance(other, Bus) and other.width == 32:
            cin = NativeWire(LogicState.LOW)
            cout = NativeWire()
            sum_bus = NativeBus32()
            adder = NativeRippleCarryAdder32(self._native, other._native, cin, sum_bus, cout)
            adder.evaluate()
            return Bus(width=32, _native=sum_bus)
        else:
            other_val = other.value if isinstance(other, Bus) else int(other)
            return Bus(width=self._width, value=(self.value + other_val))

    def __and__(self, other: Union["Bus", int]) -> "Bus":
        other_val = other.value if isinstance(other, Bus) else int(other)
        return Bus(width=self._width, value=(self.value & other_val))

    def __or__(self, other: Union["Bus", int]) -> "Bus":
        other_val = other.value if isinstance(other, Bus) else int(other)
        return Bus(width=self._width, value=(self.value | other_val))

    def __xor__(self, other: Union["Bus", int]) -> "Bus":
        other_val = other.value if isinstance(other, Bus) else int(other)
        return Bus(width=self._width, value=(self.value ^ other_val))

    def __int__(self) -> int:
        return self.value

    def __hex__(self) -> str:
        return hex(self.value)

    def __repr__(self) -> str:
        hex_digits = (self._width + 3) // 4
        return f"<Bus({self._width}) value=0x{self.value:0{hex_digits}X}>"
