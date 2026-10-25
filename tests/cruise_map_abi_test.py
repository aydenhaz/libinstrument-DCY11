"""Compare original map call registers/stack with freshly linked ARM adapters."""
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
fragment,mapview,layer,table,layer_table,data=0x90000,0x91000,0x92000,0x93000,0x94000,0x95000
sp=0x180000
xy=struct.pack('<dd',12.5,-23.25)
def machine(path):
 m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x200000)
 e=ELF32(path)
 for kind,offset,address,_,size,*_ in e.segments:
  if kind==1:m.mem_write(address,e.data[offset:offset+size])
 for address,value in ((fragment+0x3c0,mapview),(mapview,table),(layer,layer_table),(table+0x1a8,0x1d0001),(table+0x2cc,0x1d0101),(layer_table+8,0x1d0201),(sp+0x68,fragment),(sp+0xf0,data)):
  m.mem_write(address,struct.pack('<I',value))
 m.mem_write(data+0x18,xy);m.reg_write(UC_ARM_REG_SP,sp);m.reg_write(UC_ARM_REG_LR,0x1f0001)
 m.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);m.reg_write(UC_ARM_REG_FPEXC,0x40000000)
 trace=[]
 def hook(e,a,size,_):
  if a==0x1d0000:
   assert e.reg_read(UC_ARM_REG_R0)==mapview
   x=struct.pack('<II',e.reg_read(UC_ARM_REG_R2),e.reg_read(UC_ARM_REG_R3))
   stack=bytes(e.mem_read(e.reg_read(UC_ARM_REG_SP),24))
   assert x==xy[:8] and stack==xy[8:]+struct.pack('<4I',0,0,0,1)
   trace.append('go')
  elif a==0x1d0100:
   assert e.reg_read(UC_ARM_REG_R0)==mapview
   trace.append('layer');e.reg_write(UC_ARM_REG_R0,layer)
  elif a==0x1d0200:
   assert e.reg_read(UC_ARM_REG_R0)==layer and e.reg_read(UC_ARM_REG_R1)==sp+0xe8
   trace.append('car')
  else:return
  e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
 m.hook_add(UC_HOOK_CODE,hook)
 return m,trace
m,trace=machine(root/'reference/v21/libInstrumentPannelLarge.so')
m.emu_start(0x3eded,0x3ee2e,count=100);assert trace==['go','layer','car']
m,trace=machine(args.so)
m.reg_write(UC_ARM_REG_R0,mapview)
xwords=struct.unpack('<II',xy[:8]);m.reg_write(UC_ARM_REG_R2,xwords[0]);m.reg_write(UC_ARM_REG_R3,xwords[1]);m.mem_write(sp,xy[8:])
m.emu_start(symbols['_ZN7IPL_ABI12_GLOBAL__N_14goToEPvdd']|1,0x1f0000,count=200)
assert m.reg_read(UC_ARM_REG_PC)==0x1f0000
m.reg_write(UC_ARM_REG_R0,mapview);m.reg_write(UC_ARM_REG_R1,sp+0xe8);m.reg_write(UC_ARM_REG_LR,0x1f0001)
m.emu_start(symbols['_ZN7IPL_ABI12_GLOBAL__N_16setCarEPvRKN2bl6CarLocE']|1,0x1f0000,count=200)
assert m.reg_read(UC_ARM_REG_PC)==0x1f0000 and trace==['go','layer','car']
print('Original and rebuilt ARM map adapter register/stack arguments: PASS')
