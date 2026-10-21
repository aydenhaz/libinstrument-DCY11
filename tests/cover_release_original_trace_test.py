#!/usr/bin/env python3
"""Original V21 release instruction traces; external services are intercepted."""
import runpy, struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR, UC_ARM_REG_PC
ctx=runpy.run_path(str(Path(__file__).with_name('cover_original_differential_test.py')))
mu,obj,word,invoke=map(ctx.get,('mu','obj','word','invoke'))
config_stub=0x304000
mu.mem_write(0x6e934,struct.pack('<I',config_stub|1))
strings,trace={},[]
mutate=False

def utf16(address):
    out=''
    for i in range(0,256,2):
        c=struct.unpack('<H',mu.mem_read(address+i,2))[0]
        if not c:return out
        out+=chr(c)
    raise AssertionError('unterminated string')

def hook(emu,address,size,data):
    if address not in (0x181b0,0x1625a,0x1629a,config_stub,0x13bfc,0x13e24,0x13e30,0x13e3c,0x17c66,0x13e48,0x13ba8):return
    r0,r1=emu.reg_read(UC_ARM_REG_R0),emu.reg_read(UC_ARM_REG_R1)
    if address==0x181b0:
        assert r0==obj+0x18 and r1==0
        emu.mem_write(r0,b'\0'*4)
    elif address==0x1625a:strings[r0]=utf16(r1)
    elif address==0x1629a:del strings[r0]
    elif address==config_stub:
        trace.append(strings[r0]+'='+strings[r1])
        assert emu.mem_read(obj+0x70,1)==b'\0'
        assert emu.mem_read(obj+0x88,8)==b'\0'*8
        if mutate:word(0x9c,0)
    elif address in (0x13bfc,0x13e30):emu.reg_write(UC_ARM_REG_R0,0x105000)
    elif address==0x13e24:
        assert r1==obj;trace.append('remove')
    elif address==0x13e3c:
        assert r1==0;trace.append('clear-host')
    elif address==0x17c66:
        assert r0==0x6f00c;trace.append('clear-private')
    elif address==0x13e48:
        assert r0==0x106000;trace.append('image-dtor')
    else:
        assert r0==0x106000;trace.append('image-delete')
    emu.reg_write(UC_ARM_REG_PC,emu.reg_read(UC_ARM_REG_LR))
mu.hook_add(UC_HOOK_CODE,hook)
for image,enabled,optional,mutate in [(0,1,1,False),(1,0,1,False),(1,1,0,False),(1,1,1,False),(1,1,1,True)]:
    mu.mem_write(obj,bytes([0x55])*0xa8)
    word(0x9c,0x106000 if image else 0)
    mu.mem_write(obj+0x70,bytes([enabled]));mu.mem_write(obj+0x14,bytes([optional]))
    before=bytes(mu.mem_read(obj,0xa8));trace.clear()
    invoke(0x18078,obj)
    after=bytes(mu.mem_read(obj,0xa8))
    if not(image and enabled):assert before==after and not trace;continue
    expected=['widgetCoverR=127','widgetCoverG=127','widgetCoverB=127']
    if not mutate:expected+=['remove']+(['clear-host','clear-private'] if optional else [])+['image-dtor','image-delete']
    assert trace==expected,(trace,expected)
    cleared=set(range(0xc,0x14))|set(range(0x18,0x1c))|{0x70,0xa0}|set(range(0x88,0x90))|set(range(0x9c,0xa0))
    for i in range(0xa8):assert after[i]==(0 if i in cleared else before[i]),hex(i)
    assert not strings
print('PASS: 5 original cleanup traces, guards, callback ordering, external mutation and preserved bytes; host services mocked')
