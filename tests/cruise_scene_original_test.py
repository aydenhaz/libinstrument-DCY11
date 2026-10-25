"""Execute original GetNaviType with all guide/service/mode combinations."""
from pathlib import Path
import struct
import sys
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from inspect_packet_factory import ELF32
root = Path(__file__).resolve().parents[1]
elf = ELF32(root / 'reference/v21/libInstrumentPannelLarge.so')
for g in (0,1):
 for s in (0,1):
  for a in (0,1):
   for b in (0,1):
    m=Uc(UC_ARCH_ARM, UC_MODE_THUMB); m.mem_map(0,0x200000)
    for kind, offset, address, _, size, *_ in elf.segments:
     if kind == 1: m.mem_write(address,elf.data[offset:offset+size])
    guide, service, table=0x90000,0x91000,0x92000
    m.mem_write(guide,struct.pack('<I',table));m.mem_write(service,struct.pack('<I',table))
    for offset,target in ((0x20,0x93001),(0xac,0x94001),(0xc4,0x95001)):
     m.mem_write(table+offset,struct.pack('<I',target))
    calls=[]
    def hook(e,address,size,_):
     values={0x13f14:guide if g else 0,0x93000:service if s else 0,0x94000:a,0x95000:b,0x13c14:0}
     if address not in values:return
     if address!=0x13c14:calls.append(address)
     e.reg_write(UC_ARM_REG_R0,values[address]);e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
    m.hook_add(UC_HOOK_CODE,hook);m.reg_write(UC_ARM_REG_SP,0x180000);m.reg_write(UC_ARM_REG_LR,0x190001)
    m.emu_start(0x417f9,0x190000,count=200)
    assert m.reg_read(UC_ARM_REG_PC)==0x190000
    assert m.reg_read(UC_ARM_REG_R0)==(2 if not g or not s else 0 if a else 1 if b else 2)
    expected=[0x13f14]
    if g:
     expected.append(0x93000)
     if s:
      expected.append(0x94000)
      if not a:expected.append(0x95000)
    assert calls==expected
print('Original scene selector: 16 result/short-circuit cases PASS')
# Pin the VFP conversion semantics used for the first CarLoc word.
for value, expected in ((17.9,17),(-17.9,-17),(float('inf'),0x7fffffff),
                        (-float('inf'),0x80000000),(float('nan'),0)):
 m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x10000)
 m.mem_write(0x1000,elf.at(0x3ec82,4));m.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);m.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 m.reg_write(UC_ARM_REG_S0,struct.unpack('<I',struct.pack('<f',value))[0])
 m.emu_start(0x1001,0x1004,count=1)
 assert m.reg_read(UC_ARM_REG_S0)==expected & 0xffffffff
print('Original VFP speed truncation/saturation/NaN: PASS')
