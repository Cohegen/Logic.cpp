import os
import sys

# On Windows, register package directory and MSYS2 ucrt64 for dependent DLLs
if sys.platform == "win32":
    current_dir = os.path.dirname(os.path.abspath(__file__))
    try:
        os.add_dll_directory(current_dir)
    except (AttributeError, OSError):
        pass

    msys_bin = r"C:\msys64\ucrt64\bin"
    if os.path.isdir(msys_bin):
        try:
            os.add_dll_directory(msys_bin)
        except (AttributeError, OSError):
            pass

from ._pylogic_core import (
    LogicState,
    NativeWire,
    NativeClock,
    NativeBus2,
    NativeBus3,
    NativeBus4,
    NativeBus8,
    NativeBus16,
    NativeBus32,
    NativeBus64,
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
    NativeRippleCarryAdder32,
    NativeALU32,
)

from .wire import Wire
from .bus import Bus
from .gates import (
    AND,
    OR,
    XOR,
    NOT,
    NAND,
    NOR,
    XNOR,
    Buffer,
    HalfAdder,
    FullAdder,
)

Clock = NativeClock

__all__ = [
    "LogicState",
    "Wire",
    "Bus",
    "Clock",
    "AND",
    "OR",
    "XOR",
    "NOT",
    "NAND",
    "NOR",
    "XNOR",
    "Buffer",
    "HalfAdder",
    "FullAdder",
    "NativeALU32",
    "NativeRippleCarryAdder32",
]
