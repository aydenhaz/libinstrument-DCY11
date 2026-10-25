"""Execute original Cruise registration prefix with ALC enabled and disabled.
External calls are mocked; original stack save/restore and address math run.
"""
from pathlib import Path
import struct
import sys
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
root = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(root / 'tools'))
from inspect_packet_factory import ELF32
elf = ELF32(root / 'reference/v21/libInstrumentPannelLarge.so')
for logging in (False, True):
    m = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    m.mem_map(0, 0x200000)
    for kind, offset, address, _, size, *_ in elf.segments:
        if kind == 1:
            m.mem_write(address, elf.data[offset:offset + size])
    fragment, parent, alc, table, notify = 0x90000, 0x91000, 0x92000, 0x93000, 0x94000
    m.mem_write(alc, struct.pack('<I', table))
    m.mem_write(table, struct.pack('<I', 0x95001))
    trace = []
    def hook(e, address, size, _):
        if address in (0x3eb60, 0x3eb76):
            e.reg_write(UC_ARM_REG_R0, alc if logging else 0)
        elif address == 0x3ebaa:
            assert logging
            assert e.reg_read(UC_ARM_REG_R4) == fragment
            sp = e.reg_read(UC_ARM_REG_SP)
            assert struct.unpack('<I', e.mem_read(sp + 8, 4))[0] == fragment + 0x30c
            trace.append('log')
        elif address == 0x3ebb0:
            assert e.reg_read(UC_ARM_REG_R0) == fragment
            assert e.reg_read(UC_ARM_REG_R1) == parent
            e.reg_write(UC_ARM_REG_R0, 0x96000)
            trace.append('base')
        elif address == 0x3ebb4:
            e.reg_write(UC_ARM_REG_R0, notify)
        elif address == 0x3ebbc:
            assert e.reg_read(UC_ARM_REG_R0) == notify
            assert e.reg_read(UC_ARM_REG_R1) == fragment + 0x2e4
            assert e.reg_read(UC_ARM_REG_R1) < fragment + 0x3dc
            trace.append('add')
            e.emu_stop()
            return
        else:
            return
        e.reg_write(UC_ARM_REG_PC, (address + size) | 1)
    m.hook_add(UC_HOOK_CODE, hook)
    # Enter after prologue/VFP saves with equivalent reserved stack space.
    m.reg_write(UC_ARM_REG_SP, 0x180000)
    m.reg_write(UC_ARM_REG_R7, 0x181000)
    m.reg_write(UC_ARM_REG_R0, fragment)
    m.reg_write(UC_ARM_REG_R1, parent)
    m.emu_start(0x3eb4d, 0x3ebc0, count=200)
    assert trace == (['log'] if logging else []) + ['base', 'add'], trace
print('Original cruise observer receiver = Fragment+0x2e4 with ALC enabled/disabled: PASS')
