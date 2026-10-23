#!/usr/bin/env python3
"""Compare a linked Android module class with the original V21 ARM32 ABI."""
import argparse
from pathlib import Path
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from inspect_packet_factory import ELF32, plt_symbol, thumb_bl_target

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--so", required=True, type=Path, help="linked Android ARM32 development SO")
args = parser.parse_args()
root = Path(__file__).resolve().parents[1]
original = ELF32(root / "reference/v21/libInstrumentPannelLarge.so")
current = ELF32(args.so)
name = "N24NS_InstrumentPannelLarge15LargeScreenImplE"
typeinfo = "_ZTI" + name
vtable = "_ZTV" + name


def dynsym(elf):
    section = elf.sections[".dynsym"]
    names = elf.dynsym_names()
    result = {}
    for index, symbol in enumerate(names):
        address, size = struct.unpack_from("<II", elf.data, section[4] + index * 16 + 4)
        if symbol and address:
            result[symbol] = address, size
    return result


orig_table, orig_rtti = 0x6B79C, 0x6B7C8
assert [original.u32(orig_table + 4 * i) for i in (2, 3, 4, 5, 8, 9, 10)] == [
    0x2DFA5, 0x2E02D, 0x2E03D, 0x2E0F9, 0x2EAED, 0x2EAF3, 0x2EAF9]
assert original.u32(orig_table + 4) == orig_rtti
assert original.u32(orig_table + 0x1c) == orig_rtti
assert original.at(0x2EBEA, 2) == b"\x10\x20"  # new(0x10)
assert 0x2EBE8 + original.u32(0x2EEBC) == 0x6FD4C  # separate module singleton
assert original.at(0x2EC10, 4) == original.at(0x2DFC2, 4)  # ID 0x1f40
assert plt_symbol(original, thumb_bl_target(original, 0x2EC16) & ~3)[1] == (
    "_ZN15ExModuleManager9ModuleRegE17ModuleInterfaceIdPv")
assert plt_symbol(original, thumb_bl_target(original, 0x2DFC6) & ~3)[1] == (
    "_ZN15ExModuleManager11ModuleUnRegE17ModuleInterfaceId")
assert original.at(0x2E090, 4) == b"\x09\xf1\x04\x00"  # r0 = this+4
assert original.at(0x2E0D4, 4) == b"\x4f\xf0\xff\x36"  # null-out -> -1
orig_relocs = original.relocation_symbols(".rel.dyn")
assert orig_relocs[orig_rtti + 0x10] == (2, "_ZTI13IBaseModuleEx")
assert orig_relocs[orig_rtti + 0x18] == (2, "_ZTI22IMultiScreenInterfaces")
assert (original.u32(orig_rtti + 0x14), original.u32(orig_rtti + 0x1c)) == (2, 0x402)

symbols = dynsym(current)
assert symbols[vtable][1] == 0x2c, symbols[vtable]
assert symbols[typeinfo][1] == 0x20, symbols[typeinfo]
assert symbols["_ZTI13IBaseModuleEx"][1] == 8
assert symbols["_ZTI22IMultiScreenInterfaces"][1] == 8
table, rtti = symbols[vtable][0], symbols[typeinfo][0]
relocs = current.relocation_symbols(".rel.dyn")
assert current.u32(table) == 0
assert struct.unpack("<i", current.at(table + 0x18, 4))[0] == -4
assert relocs[table + 4] == (2, typeinfo)
assert relocs[table + 0x1c] == (2, typeinfo)
prefix = "_Z" + name[:-1]
thunk = "_ZThn4_" + name[:-1]
for offset, symbol in (
    (0x08, prefix + "D1Ev"), (0x0c, prefix + "D0Ev"),
    (0x10, prefix + "14QueryInterfaceEiPPv"),
    (0x14, prefix + "15doStartFragmentEiPN9GNS_FRAME10CGFragmentERKNS1_8CGIntentE"),
    (0x20, thunk + "D1Ev"), (0x24, thunk + "D0Ev"),
    (0x28, thunk + "15doStartFragmentEiPN9GNS_FRAME10CGFragmentERKNS1_8CGIntentE"),
):
    assert relocs[table + offset] == (2, symbol), (offset, relocs.get(table + offset))
assert relocs[rtti + 0x10] == (2, "_ZTI13IBaseModuleEx")
assert relocs[rtti + 0x18] == (2, "_ZTI22IMultiScreenInterfaces")
assert (current.u32(rtti + 0x14), current.u32(rtti + 0x1c)) == (2, 0x402)
print("LargeScreenImpl dual-interface ARM32 vtable/RTTI: PASS")
