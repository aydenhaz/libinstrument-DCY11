#!/usr/bin/env python3
"""Execute original capture callback through its configuration writes.
External String16 formatting/construction/destruction and configuration storage
are intercepted, so this validates call sequence/arguments, not host services.
"""
import runpy
from pathlib import Path
import struct
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_LR, UC_ARM_REG_PC
ctx=runpy.run_path(str(Path(__file__).with_name('cover_rgb_original_differential_test.py')))
mu,obj,word=ctx['mu'],ctx['obj'],ctx['word']
invoke=ctx['context']['invoke']
format_stub,config_stub=0x303000,0x304000
mu.mem_write(0x6e930,struct.pack('<I',format_stub|1))
mu.mem_write(0x6e934,struct.pack('<I',config_stub|1))
strings,trace={},[]
def utf16(address):
    result=''
    for offset in range(0,512,2):
        unit=struct.unpack('<H',mu.mem_read(address+offset,2))[0]
        if not unit:return result
        result+=chr(unit)
    raise AssertionError('unterminated UTF16 literal')
def hook(emu,address,size,data):
    if address not in (format_stub,config_stub,0x1625a,0x1629a):return
    r0,r1=emu.reg_read(UC_ARM_REG_R0),emu.reg_read(UC_ARM_REG_R1)
    if address==format_stub:
        assert utf16(r1)=='%d'
        value=emu.reg_read(UC_ARM_REG_R2)
        if value&0x80000000:value-=0x100000000
        strings[r0]=str(value);trace.append('format:'+str(value))
    elif address==config_stub:
        trace.append(strings[r0]+'='+strings[r1])
    elif address==0x1625a:
        strings[r0]=utf16(r1)
    else:
        del strings[r0]
    emu.reg_write(UC_ARM_REG_PC,emu.reg_read(UC_ARM_REG_LR))
mu.hook_add(UC_HOOK_CODE,hook)
data=bytes([0,127,255,31,0,127,255,42])
mu.mem_write(ctx['data_address'],data)
word(0x80,2);word(0x84,1);word(0x88,ctx['data_address']);word(0x8c,8)
mu.mem_write(obj+0x7c,b'\x04');mu.mem_write(obj+0x98,b'\x01');mu.mem_write(obj+0xa0,b'\x00')
assert invoke(0x17e9c,obj,23)==1
assert trace==['format:0','format:127','format:255','widgetCoverR=0','widgetCoverG=127','widgetCoverB=255'],trace
assert not strings,'all temporary String16 values must be destroyed'
for event in (-1,22,24):
    trace.clear();mu.mem_write(obj+0x98,b'\x01');mu.mem_write(obj+0xa0,b'\x00')
    assert invoke(0x17e9c,obj,event)==1
    assert not trace and mu.mem_read(obj+0x98,1)==b'\x01' and mu.mem_read(obj+0xa0,1)==b'\x00'
print('PASS: full original callback RGB config trace, temporary cleanup, and three ignored events; services mocked')
