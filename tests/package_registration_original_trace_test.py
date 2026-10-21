#!/usr/bin/env python3
"""Original package registration initializer, host services intercepted."""
import runpy,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3,UC_ARM_REG_LR,UC_ARM_REG_PC
c=runpy.run_path(str(Path(__file__).with_name('cover_original_differential_test.py')))
m,invoke=c['mu'],c['invoke'];strings={};trace=[]
m.mem_write(0x6e92c,struct.pack('<I',0x305001))
def utf16(a):
 out=''
 for i in range(0,512,2):
  n=int.from_bytes(m.mem_read(a+i,2),'little')
  if not n:return out
  out+=chr(n)
 raise AssertionError('unterminated literal')
def hook(e,a,n,d):
 if a not in (0x13e00,0x13d58,0x13e0c,0x139ec):return
 r0,r1,r2,r3=[e.reg_read(r) for r in (UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3)]
 if a==0x13e00:strings[r0]=utf16(r1);trace.append('string:'+strings[r0])
 elif a==0x13d58:trace.append('destroy:'+strings.pop(r0))
 elif a==0x13e0c:
  assert (r0,r2)==(0x6fd28,0x2eb09)
  assert strings[r1]=='CGIPLPacket' and strings[r3]=='com.autonavi.package.GIPLPacket'
  trace.append('register')
 else:
  assert (r0,r1,r2)==(0x305001,0x6fd28,0x68be8),(hex(r0),hex(r1),hex(r2))
  trace.append('atexit')
 e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
m.hook_add(UC_HOOK_CODE,hook);invoke(0x14e40)
assert trace==['string:CGIPLPacket','string:com.autonavi.package.GIPLPacket','register','destroy:com.autonavi.package.GIPLPacket','destroy:CGIPLPacket','atexit'] and not strings
print('PASS: original package class/alias, Thumb factory pointer, node storage and destructor registration ordering; host calls mocked')
