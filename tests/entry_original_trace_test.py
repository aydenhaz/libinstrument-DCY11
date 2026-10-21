#!/usr/bin/env python3
"""Execute original startup/exit with host side effects mocked, logging off."""
import runpy,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_SP,UC_ARM_REG_LR,UC_ARM_REG_PC
c=runpy.run_path(str(Path(__file__).with_name('cover_original_differential_test.py')))
m,invoke=c['mu'],c['invoke']
simulation,manager,service=0x105000,0x106000,0x107000
trace=[];available=True;created=False;load_result=0
strings={};intent=None

def text(a,wide=False):
 out='';step=2 if wide else 1
 for i in range(0,1024,step):
  n=int.from_bytes(m.mem_read(a+i,step),'little')
  if not n:return out
  out+=chr(n)
 raise AssertionError('unterminated string')
def hook(e,a,n,d):
 global created,intent
 if a not in (0x13c2c,0x147d8,0x147e4,0x147f0,0x13e00,0x13c5c,0x147fc,0x14808,0x13ce0,0x13d58,0x14814):return
 r0,r1,r2,r3=[e.reg_read(r) for r in (UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3)]
 if a==0x13c2c:trace.append('service');e.reg_write(UC_ARM_REG_R0,service if available else 0)
 elif a==0x147d8:assert r0==service;trace.append('created?');e.reg_write(UC_ARM_REG_R0,int(created))
 elif a==0x147e4:assert r0==service and r1==1;created=True;trace.append('created=1')
 elif a==0x147f0:assert r0==simulation;trace.append('manager');e.reg_write(UC_ARM_REG_R0,manager)
 elif a==0x13e00:strings[r0]=text(r1,True);assert strings[r0]=='InstrumentPannelLargePackage';trace.append('name')
 elif a==0x13c5c:intent=r0;trace.append('intent')
 elif a==0x147fc:assert r0==intent;trace.append(text(r1,True)+'='+text(r2))
 elif a==0x14808:
  assert r0==manager and strings[r1]=='InstrumentPannelLargePackage' and r2==intent and r3==0
  assert bytes(e.mem_read(e.reg_read(UC_ARM_REG_SP),4))==b'\0'*4
  trace.append('load');e.reg_write(UC_ARM_REG_R0,load_result)
 elif a==0x14814:assert r0==manager and strings[r1]=='InstrumentPannelLargePackage' and r2==0;trace.append('unload')
 elif a==0x13ce0:assert r0==intent;intent=None;trace.append('destroy-intent')
 else:del strings[r0];trace.append('destroy-name')
 e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
m.hook_add(UC_HOOK_CODE,hook)
tail=['manager','name','intent','fromPackage=instrumentPannelLargePackage','packageType=com.autonavi.package.GIPLPacket','load','destroy-intent','destroy-name']
for available,created,load_result in [(True,False,0),(True,False,1),(False,False,0),(True,True,0)]:
 trace.clear();initial=created
 result=invoke(0x312c4,simulation)
 expected=['service','created?'] if initial else (['service','created?','created=1'] if available else ['service'])+tail
 assert trace==expected,(trace,expected)
 assert result==(0 if initial else 1) and not strings and intent is None
trace.clear();invoke(0x313c8,simulation)
assert trace==['manager','name','unload','destroy-name'] and not strings
print('PASS: original startup/exit receiver, exact literals, flags, cleanup order, service-null/duplicate branches, and ignored load result; host services mocked')
