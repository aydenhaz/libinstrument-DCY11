#!/usr/bin/env python3
"""Execute cached V21 host activity lookup and package context getter.
Only object-validity checking and workstation lookup are mocked. Lazy creation
and abort branches are intentionally outside this regression.
"""
import hashlib,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import Uc,UC_ARCH_ARM,UC_MODE_THUMB,UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
path=Path(sys.argv[1]);assert hashlib.sha256(path.read_bytes()).hexdigest()=='45252a215427ba52fd0165417de517ee835d079a31bc8eaa8e68e04d841bde35'
m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x500000);m.mem_map(0x500000,0x300000)
with path.open('rb') as f:
 for s in ELFFile(f).iter_segments():
  if s['p_type']=='PT_LOAD':m.mem_write(s['p_vaddr'],s.data())
package,station,simulation,activity=0x500000,0x501000,0x502000,0x503000
write=lambda a,v:m.mem_write(a,struct.pack('<I',v))
trace=[]
def hook(e,a,n,d):
 if a==0x10c96c:trace.append(e.reg_read(UC_ARM_REG_R0))
 elif a==0x10c768:e.reg_write(UC_ARM_REG_R0,station)
 elif a==0x112de0:
  assert e.reg_read(UC_ARM_REG_R0)==0x504000
  trace.append(0x504000);e.reg_write(UC_ARM_REG_R0,0xf2345678)
 elif a==0x112f6c:
  assert e.reg_read(UC_ARM_REG_R0)==station
  trace.append(station);e.reg_write(UC_ARM_REG_R0,0x505000)
 elif a==0x11095c:
  # Resolve this PLT entry to the real host implementation, preserving LR/r0.
  e.reg_write(UC_ARM_REG_PC,0x21a135);return
 else:return
 e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
m.hook_add(UC_HOOK_CODE,hook)
def invoke(entry,receiver):
 m.reg_write(UC_ARM_REG_R0,receiver);m.reg_write(UC_ARM_REG_SP,0x608000);m.reg_write(UC_ARM_REG_LR,0x700001)
 m.emu_start(entry|1,0x700000,count=500)
 assert m.reg_read(UC_ARM_REG_PC)==0x700000
 return m.reg_read(UC_ARM_REG_R0)
write(package+0x68,station);write(station+0x138,simulation);write(simulation+0x1fc,activity)
write(package+0x5c,0x12345678);write(package+0x64,0x87654321)
assert invoke(0x1b6bc8,package)==0x12345678 # Not constructor's +0x64 context member.
assert invoke(0x21a134,simulation)==activity and trace==[simulation+0x54]
trace.clear()
assert invoke(0x1b71e4,package)==activity and trace==[package+0x40,simulation+0x54]
print('PASS: host package context +0x5c, cached simulation activity +0x1fc, and package -> workstation -> simulation -> activity chain; validity checks mocked')

# Thread getters require a simulation receiver and its adapter at +0x218.
write(simulation+0x180,0x13572468);write(simulation+0x218,0x504000);write(0x504000+0x34,0x24681357)
trace.clear();assert invoke(0x216c16,simulation)==0x13572468 and trace==[simulation+0x54]
trace.clear();assert invoke(0x2187c0,simulation)==0x24681357 and trace==[]
assert invoke(0x2182d4,simulation)==0xf2345678 and trace==[0x504000]
print('PASS: original simulation ID, executor ID and render-thread adapter receiver; adapter call mocked, missing-adapter abort path untested')

write(simulation+0x17c,station);trace.clear()
assert invoke(0x21d090,simulation)==0x505000 and trace==[station]
write(simulation+0x200,0x506000);write(0x506004,0xdeadbeef);trace.clear()
assert invoke(0x21a240,simulation)==0x506000
assert struct.unpack('<I',m.mem_read(0x506004,4))[0]==activity and trace==[simulation+0x54]
print('PASS: package-manager workstation forwarding and exhibition activity refresh; manager method mocked')
