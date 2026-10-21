#!/usr/bin/env python3
"""V21 setCoverUtil instruction traces with intercepted host interfaces."""
import runpy,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_SP,UC_ARM_REG_PC,UC_ARM_REG_LR
c=runpy.run_path(str(Path(__file__).with_name('cover_original_differential_test.py')))
mu,invoke=c['mu'],c['invoke']
fragment,cover,facility,other,service,ft,st=0x105000,0x106000,0x107000,0x107100,0x108000,0x109000,0x10a000
trace=[]
service_on=facility_on=mutate=False
pack=lambda x:struct.pack('<I',x)
read=lambda a:struct.unpack('<I',mu.mem_read(a,4))[0]
mu.mem_write(facility,pack(ft));mu.mem_write(other,pack(ft));mu.mem_write(service,pack(st))
mu.mem_write(ft+0xa8,pack(0x305001));mu.mem_write(ft+0x24,pack(0x305201));mu.mem_write(st+0x20,pack(0x305101))
config=bytes(range(80));mu.mem_write(fragment+0x3a0,config)
def hook(e,a,n,d):
 if a not in (0x13cd4,0x305000,0x305100,0x13fd4,0x305200,0x1de64,0x18264,0x13d58):return
 r0,r1,r2,r3=[e.reg_read(r) for r in (UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3)]
 if a==0x13cd4:trace.append(1);e.reg_write(UC_ARM_REG_R0,service if service_on else 0)
 elif a==0x305000:assert r0==facility;trace.append(2);e.reg_write(UC_ARM_REG_R0,0xfedcba98)
 elif a==0x305100:assert r0==service and r1==0xfedcba98;trace.append(3);e.reg_write(UC_ARM_REG_R0,0)
 elif a==0x13fd4:
  assert r0==fragment;trace.append(4);e.reg_write(UC_ARM_REG_R0,0x1230)
  if mutate:mu.mem_write(fragment+0x388,pack(other));mu.mem_write(fragment+0x40c,pack(0))
 elif a==0x305200:assert r0==(other if mutate else facility);trace.append(5);e.reg_write(UC_ARM_REG_R0,0x4560)
 elif a==0x1de64:assert r1==fragment+0x3a0;mu.mem_write(r0,bytes(mu.mem_read(r1,80)))
 elif a==0x18264:
  assert (r0,r1,r2,r3)==(cover,0x1230,0,0x4560)
  assert bytes(mu.mem_read(read(e.reg_read(UC_ARM_REG_SP)),80))==config
  trace.append(6)
 e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
mu.hook_add(UC_HOOK_CODE,hook)
for target,service_on,facility_on,mutate in [(0,True,True,False),(cover,False,True,False),(cover,True,False,False),(cover,True,True,False),(cover,True,True,True)]:
 trace.clear();mu.mem_write(fragment+0x388,pack(facility if facility_on else 0))
 invoke(0x3bdbc,fragment,target)
 expected=[] if not target else [1] if not(service_on and facility_on) else [1,2,3,4,5,6]
 assert trace==expected,trace
 assert read(fragment+0x40c)==(0 if mutate else target)
print('PASS: 5 original Opening Cover binding traces: guards, ABI arguments, configuration copy and reentrant field reloads; host services mocked')
