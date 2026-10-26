"""Verify broadcast message-reference signature and original/rebuilt dispatch."""
import argparse
from pathlib import Path
import struct
import subprocess
import sys
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(root/'tools'))
from inspect_packet_factory import ELF32
p=argparse.ArgumentParser();p.add_argument('--so',type=Path,required=True);p.add_argument('--nm',required=True);args=p.parse_args()
symbols={line.split()[2]:int(line.split()[0],16) for line in subprocess.check_output([args.nm,'-n',str(args.so)],text=True).splitlines() if len(line.split())==3 and line.split()[1] not in ('U','w','v')}
original=ELF32(root/'reference/v21/libInstrumentPannelLarge.so')
rtti=original.u32(0x68f5c-4);name=original.u32(rtti+4)
assert original.at(name,200).split(b'\0')[0]==b'N3GPI7Closure8CallableIFvRKN9GNS_FRAME16STGBroadcastMsg_EEEE'
assert tuple(original.u32(0x6df34+i*4) for i in range(4))==(0x155b5,0x41441,0x41445,0x41461)
def run(rebuilt,virtual,adjustment,offset):
 elf=ELF32(args.so) if rebuilt else original
 m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x200000)
 for kind,off,addr,_,size,*_ in elf.segments:
  if kind==1:m.mem_write(addr,elf.data[off:off+size])
 if rebuilt:
  for got,(_,symbol) in elf.relocation_symbols('.rel.plt').items():
   if symbol in symbols:m.mem_write(got,struct.pack('<I',symbols[symbol]|1))
 wrapper,target,table,message,callback=0x90000,0x91040,0x93000,0x94000,0x95001
 receiver=target+adjustment
 m.mem_write(receiver,struct.pack('<I',table))
 m.mem_write(table+offset,struct.pack('<I',callback))
 member=offset if virtual else callback
 m.mem_write(wrapper,struct.pack('<IIIi',0,target,member,adjustment*2+int(virtual)))
 m.mem_write(message,bytes(range(64)))
 seen=[]
 def hook(e,address,size,_):
  if address==callback-1:
   seen.append((e.reg_read(UC_ARM_REG_R0),e.reg_read(UC_ARM_REG_R1)))
   assert seen[-1]==(receiver,message)
   e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
 m.hook_add(UC_HOOK_CODE,hook)
 m.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);m.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 m.reg_write(UC_ARM_REG_R0,wrapper);m.reg_write(UC_ARM_REG_R1,message)
 m.reg_write(UC_ARM_REG_SP,0x180000);m.reg_write(UC_ARM_REG_LR,0x1f0001)
 entry=next(a for n,a in symbols.items() if 'BroadcastMemberWrapper6invoke' in n) if rebuilt else 0x41444
 m.emu_start(entry|1,0x1f0000,count=300)
 assert m.reg_read(UC_ARM_REG_PC)==0x1f0000 and len(seen)==1
 assert bytes(m.mem_read(message,64))==bytes(range(64))
 return seen
cases=0
for virtual in (False,True):
 for adjustment in (-8,0,4):
  for offset in (0,0xe4,0xec):
   assert run(False,virtual,adjustment,offset)==run(True,virtual,adjustment,offset);cases+=1
print(f'Original broadcast signature and {cases} direct/virtual/adjusted message-reference dispatch cases: PASS')
