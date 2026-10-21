#!/usr/bin/env python3
"""Validate original Fragment getters and destructor role, not constructors."""
import runpy,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_SP,UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_LR,UC_ARM_REG_PC
c=runpy.run_path(str(Path(__file__).with_name('cover_original_differential_test.py')))
m,obj,invoke=c['mu'],c['obj'],c['invoke']
word=lambda a:struct.unpack('<I',m.mem_read(a,4))[0]
assert [word(0x6bcf0+i*4) for i in range(4)]==[0x31475,0x31529,0x156e1,0x156e5]
assert word(0x6be50)==0xffffffec and word(0x6be54)==0x6c040
assert (0x6be50-0x6bcf0)//4==88
for a,b in [(0,0),(1,0),(0,1),(127,255)]:
 m.mem_write(obj+4,bytes([a,b]));assert invoke(0x156e0,obj)==a;assert invoke(0x156e4,obj)==b
trace=[];log_object=None
signature=0x54fc7

def hook(e,a,n,d):
 global log_object
 if a not in (0x165f0,0x16674,0x4292c,0x45574,0x13b84,0x4281c):return
 r0=e.reg_read(UC_ARM_REG_R0)
 if a==0x13b84:
  assert r0==0x8a0;trace.append('allocate-0x8a0');e.reg_write(UC_ARM_REG_R0,obj)
 elif a==0x4281c:
  assert r0==obj and e.reg_read(UC_ARM_REG_R1)==1;trace.append('guide-base-ctor')
 elif a==0x165f0:
  assert 0x200000<=r0<0x210000 # stack FT, not Fragment storage
  assert e.reg_read(UC_ARM_REG_R1)==obj+0x30c and e.reg_read(UC_ARM_REG_R2)==signature
  log_object=r0;trace.append('trace-begin')
 elif a==0x16674:assert r0==log_object;trace.append('trace-end')
 elif a==0x4292c:assert r0==obj;trace.append('base-destructor')
 else:assert r0==obj;trace.append('delete')
 e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
m.hook_add(UC_HOOK_CODE,hook)
expected={0:8,0x14:0x170,0x58:0x198,0x70:0x1a8,0xb0:0x1c0,0x2e4:0x1d0,0x364:0x1dc,0x368:0x2ec,0x38c:0x30c,0x390:0x324,0x394:0x330,0x398:0x33c,0x39c:0x348,0x3a0:0x354}
for entry,deleting in [(0x31474,False),(0x31528,True)]:
 m.mem_write(obj,b'\x55'*0x440);trace.clear();invoke(entry,obj)
 assert trace==['trace-begin','trace-end','base-destructor']+(['delete'] if deleting else [])
 wanted=bytearray(b'\x55'*0x440)
 for offset,point in expected.items():wanted[offset:offset+4]=struct.pack('<I',0x6bce8+point)
 assert bytes(m.mem_read(obj,0x440))==wanted
print('PASS: 88-slot A106 primary table, raw +4/+5 getters, 14 destructor vptr resets, stack trace object and deleting-destructor ordering; callees mocked')

# The actual A106 construction is inlined in the mode-switch path, not 0x31474.
signature=0x54ec7;trace.clear();m.mem_write(obj,b'\x55'*0x8a0)
m.reg_write(UC_ARM_REG_SP,0x208000)
m.emu_start(0x2e929,0x2e9a4,count=500)
assert m.reg_read(UC_ARM_REG_PC)==0x2e9a4
assert trace==['allocate-0x8a0','guide-base-ctor','trace-begin','trace-end'],trace
wanted=bytearray(b'\x55'*0x8a0)
for offset,point in expected.items():wanted[offset:offset+4]=struct.pack('<I',0x6bce8+point)
assert bytes(m.mem_read(obj,0x8a0))==wanted
print('PASS: real inline A106 construction allocates 0x8a0, calls Guide base with mode 1, installs 14 vptrs, and constructs stack trace; allocation/base/log mocked')
