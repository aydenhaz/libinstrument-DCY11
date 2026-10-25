"""Execute V21 conversion and array growth and compare reconstructed outputs."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
root = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(root / 'tools'))
from inspect_packet_factory import ELF32
elf = ELF32(root / 'reference/v21/libInstrumentPannelLarge.so')
def machine():
    m = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    m.mem_map(0, 0x800000)
    for kind, offset, address, _, size, *_ in elf.segments:
        if kind == 1:
            m.mem_write(address, elf.data[offset:offset + size])
    m.reg_write(UC_ARM_REG_SP, 0x180000)
    m.reg_write(UC_ARM_REG_LR, 0x1f0001)
    m.reg_write(UC_ARM_REG_C1_C0_2, 0xf00000)
    m.reg_write(UC_ARM_REG_FPEXC, 0x40000000)
    return m
expected = bytearray()
for valid in (-1, 0, 1, 2):
    m = machine()
    sp, record = 0x180000, 0x190000
    source = bytearray((i * 37 + 11) & 255 for i in range(0x110))
    struct.pack_into('<i', source, 0x58, valid)
    m.mem_write(record, bytes(source))
    m.mem_write(sp + 0x70, bytes([0xa5]) * 0x68)
    m.mem_write(sp + 0x58, struct.pack('<II', sp + 0xa0, sp + 0xc8))
    m.reg_write(UC_ARM_REG_R8, record)
    def clear(e, address, size, _):
        if address == 0x13f80:
            assert e.reg_read(UC_ARM_REG_R0) == sp + 0xa0
            assert e.reg_read(UC_ARM_REG_R1) == 0x19
            e.mem_write(sp + 0xa0, bytes(0x19))
            e.reg_write(UC_ARM_REG_PC, e.reg_read(UC_ARM_REG_LR))
    m.hook_add(UC_HOOK_CODE, clear)
    m.emu_start(0x3eca9, 0x3ed2c, count=200)
    assert m.reg_read(UC_ARM_REG_PC) == 0x3ed2c
    expected.extend(m.mem_read(sp + 0x70, 0x68))
for flags in (0x11, 0x10):
    m = machine()
    array, table, entry = 0x90000, 0x91000, 0x92000
    m.mem_write(array, struct.pack('<IIIIB3x', 0, 0, 0, table, flags))
    m.mem_write(table + 8, struct.pack('<II', 0x93001, 0x94001))
    # Alternate two arenas; poison released storage to detect missing copies.
    arenas = [0x200000, 0x400000]
    live = set()
    def calls(e, address, size, _):
        if address == 0x14a48:
            dest, src, length = (e.reg_read(r) for r in (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2))
            assert length == 0x68
            e.mem_write(dest, bytes(e.mem_read(src, length)))
        elif address == 0x93000:
            assert e.reg_read(UC_ARM_REG_R0) == array + 12
            allocation = next(a for a in arenas if a not in live)
            live.add(allocation)
            e.mem_write(allocation, bytes([0xcc]) * (1500 * 0x68))
            e.reg_write(UC_ARM_REG_R0, allocation)
        elif address == 0x94000:
            assert e.reg_read(UC_ARM_REG_R0) == array + 12
            old = e.reg_read(UC_ARM_REG_R1)
            if old:
                assert old in live
                live.remove(old)
                e.mem_write(old, bytes([0xdd]) * (1500 * 0x68))
        else:
            return
        e.reg_write(UC_ARM_REG_PC, e.reg_read(UC_ARM_REG_LR))
    m.hook_add(UC_HOOK_CODE, calls)
    for i in range(1100):
        m.mem_write(entry, bytes([i & 255]) * 0x68)
        m.reg_write(UC_ARM_REG_R0, array)
        m.reg_write(UC_ARM_REG_R1, entry)
        m.reg_write(UC_ARM_REG_LR, 0x1f0001)
        m.emu_start(0x39661, 0x1f0000, count=40000)
        assert m.reg_read(UC_ARM_REG_PC) == 0x1f0000
        data, capacity, count = struct.unpack('<III', m.mem_read(array, 12))
        assert count == i + 1
        if i in (0, 5, 6, 500, 1099):
            expected.extend(struct.pack('<I', capacity))
    for i in range(1100):
        assert bytes(m.mem_read(data + i * 0x68, 0x68)) == bytes([i & 255]) * 0x68
with tempfile.TemporaryDirectory() as directory:
    binary = str(Path(directory) / 'conversion')
    subprocess.run(['g++', '-std=c++11', '-fsanitize=address,undefined',
                    '-fno-omit-frame-pointer', '-I' + str(root / 'stub_include'),
                    str(root / 'tests/location_conversion_test.cpp'),
                    str(root / 'internal/abi/LocationTypes.cpp'), '-o', binary], check=True)
    actual = subprocess.check_output([binary])
    assert actual == expected, [(i, a, b) for i, (a, b) in enumerate(zip(actual, expected)) if a != b][:20]
print('Original conversion (4 cases), 2200 appends/growth, alias and overflow checks: PASS')
