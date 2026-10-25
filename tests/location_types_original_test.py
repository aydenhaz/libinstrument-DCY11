"""Compare reconstructed LocInfo against executed V21 ARM instructions.
Requires unicorn; g++ by default. Heap/string address is normalized, not data.
"""
import argparse
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
root = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(root / 'tools'))
from inspect_packet_factory import ELF32, plt_symbol
parser = argparse.ArgumentParser()
parser.add_argument('--cxx', default='g++')
args = parser.parse_args()
elf = ELF32(root / 'reference/v21/libInstrumentPannelLarge.so')
def machine():
    m = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    m.mem_map(0, 0x200000)
    for kind, offset, address, _, size, *_ in elf.segments:
        if kind == 1:
            m.mem_write(address, elf.data[offset:offset + size])
    m.reg_write(UC_ARM_REG_SP, 0x180000)
    m.reg_write(UC_ARM_REG_LR, 0x1f0001)
    return m
with tempfile.TemporaryDirectory() as directory:
    binary = Path(directory) / 'location-test'
    subprocess.run([args.cxx, '-std=c++11', '-fsanitize=address,undefined',
                    '-fno-omit-frame-pointer', '-I' + str(root / 'stub_include'),
                    str(root / 'tests/location_types_test.cpp'),
                    str(root / 'internal/abi/LocationTypes.cpp'), '-o', str(binary)], check=True)
    for fill in (0, 0xa5, 0xff):
        native = subprocess.check_output([str(binary), str(fill)])
        m = machine()
        base = 0x90000
        m.mem_write(base - 16, bytes([fill]) * (0x690 + 32))
        calls = []
        def hook(e, address, size, _):
            if address in (0x13f80, 0x13e18, 0x13a94):
                dest, length = e.reg_read(UC_ARM_REG_R0), e.reg_read(UC_ARM_REG_R1)
                assert base <= dest and dest + length <= base + 0x690
                e.mem_write(dest, bytes(length))
            elif address == 0x18c8a:
                assert e.reg_read(UC_ARM_REG_R0) == base + 0x128
                calls.append('String16')
                e.mem_write(base + 0x128, struct.pack('<I', 0x190000))
            else:
                return
            e.reg_write(UC_ARM_REG_PC, e.reg_read(UC_ARM_REG_LR))
        m.hook_add(UC_HOOK_CODE, hook)
        m.reg_write(UC_ARM_REG_R0, base)
        m.emu_start(0x394ed, 0x1f0000, count=2000)
        assert m.reg_read(UC_ARM_REG_PC) == 0x1f0000
        actual = bytes(m.mem_read(base, 0x690))
        differences = [(hex(i), a, b) for i, (a, b) in enumerate(zip(actual, native)) if a != b]
        assert len(native) == 0x690 and not differences, differences[:30]
        assert calls == ['String16']
        assert bytes(m.mem_read(base - 16, 16)) == bytes([fill]) * 16
        assert bytes(m.mem_read(base + 0x690, 16)) == bytes([fill]) * 16
# Original inline CarLoc initialization: only data/size fields, vptr and flags.
m = machine()
sp = 0x180000
m.mem_write(sp + 0xe8, bytes([0xa5]) * 28)
def car_init(e, address, size, _):
    if address == 0x13f80:
        assert e.reg_read(UC_ARM_REG_R0) == sp + 0xe8
        assert e.reg_read(UC_ARM_REG_R1) == 20
        e.mem_write(sp + 0xe8, bytes(20))
        e.reg_write(UC_ARM_REG_PC, e.reg_read(UC_ARM_REG_LR))
m.hook_add(UC_HOOK_CODE, car_init)
m.emu_start(0x3ec2d, 0x3ec42, count=30)
assert bytes(m.mem_read(sp + 0xe8, 20)) == bytes(20)
assert bytes(m.mem_read(sp + 0x100, 4)) == bytes([0x11, 0xa5, 0xa5, 0xa5])
assert struct.unpack('<I', m.mem_read(sp + 0xfc, 4))[0] == 0x6cf70
# Four allocator slots: complete/deleting destructors, scalar new, scalar delete.
assert struct.unpack('<4I', elf.at(0x6cf70, 16)) == (0x39769, 0x3976b, 0x3976f, 0x39775)
assert plt_symbol(elf, 0x13b84)[1] == '_Znwj'
assert plt_symbol(elf, 0x13ba8)[1] == '_ZdlPv'
# Execute original array destructor for owned, borrowed, and null data.
for flags, data in ((0x11, 0x190000), (1, 0x190000), (0x11, 0)):
    m = machine()
    base, allocator_table = 0x90000, 0x91000
    m.mem_write(base, struct.pack('<IIIIB3x', data, 7, 3, allocator_table, flags))
    m.mem_write(allocator_table + 12, struct.pack('<I', 0x92001))
    releases = []
    def release(e, address, size, _):
        if address == 0x92000:
            assert e.reg_read(UC_ARM_REG_R0) == base + 12
            releases.append(e.reg_read(UC_ARM_REG_R1))
            e.reg_write(UC_ARM_REG_PC, e.reg_read(UC_ARM_REG_LR))
    m.hook_add(UC_HOOK_CODE, release)
    m.reg_write(UC_ARM_REG_R0, base)
    m.emu_start(0x39747, 0x1f0000, count=100)
    assert m.reg_read(UC_ARM_REG_PC) == 0x1f0000
    assert releases == ([data] if flags & 0x10 else [])
    assert bytes(m.mem_read(base, 12)) == bytes(12)
print('LocInfo original-byte comparison (3 fills), ownership sanitizers, original array release (3 cases): PASS')
