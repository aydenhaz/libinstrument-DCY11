#!/usr/bin/env python3
"""Compare recovered helpers with original Thumb instructions, logging disabled.
Requires pyelftools, unicorn and g++; takes original V21 SO path as argument.
This does not emulate Android or instantiate the observer class.
"""
import ctypes
import hashlib
from itertools import product
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC

path=Path(sys.argv[1])
assert hashlib.sha256(path.read_bytes()).hexdigest()=='6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3', 'Expected original V21 binary'
mu=Uc(UC_ARCH_ARM,UC_MODE_THUMB)
mu.mem_map(0,0x80000)
with path.open('rb') as stream:
    elf=ELFFile(stream)
    for segment in elf.iter_segments():
        if segment['p_type']=='PT_LOAD':
            mu.mem_write(segment['p_vaddr'],segment.data())
obj,stack,stop=0x100000,0x200000,0x300000
for address in (obj,stack,stop):mu.mem_map(address,0x10000)

def hook(emu,address,size,data):
    if address==0x13c14:  # Mock only external I_GetAlcPtr: logging unavailable.
        emu.reg_write(UC_ARM_REG_R0,0)
        emu.reg_write(UC_ARM_REG_PC,emu.reg_read(UC_ARM_REG_LR))
mu.hook_add(UC_HOOK_CODE,hook)

def word(offset,value):mu.mem_write(obj+offset,struct.pack('<I',value&0xffffffff))
def invoke(entry,*arguments):
    for register,value in zip((UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3),arguments):mu.reg_write(register,value&0xffffffff)
    mu.reg_write(UC_ARM_REG_SP,stack+0x8000)
    mu.reg_write(UC_ARM_REG_LR,stop|1)
    mu.emu_start(entry|1,stop,count=1000)
    assert mu.reg_read(UC_ARM_REG_PC)==stop,'Original function did not return'
    return mu.reg_read(UC_ARM_REG_R0)

header=Path(__file__).resolve().parents[1]/'internal/abi/CoverRenderState.h'
with tempfile.TemporaryDirectory() as directory:
    root=Path(directory)
    source=root/'helper.cpp'
    source.write_text('#include "'+str(header)+'"\nextern "C" bool finished(bool i,bool e,bool s,int32_t c,int32_t t){return IPL_ABI::coverUiFirstRenderFinished(i,e,s,c,t);}\nextern "C" int32_t changed(int32_t configured,int32_t surface,int32_t event,bool image,bool enabled,int32_t counter){IPL_ABI::coverSurfaceStatusChanged(configured,surface,event,image,enabled,counter);return counter;}\n')
    with source.open('a') as output:
        output.write('extern "C" int paint(bool image,bool enabled,bool flag,int32_t counter,int32_t threshold,bool inhibit,bool requested){auto p=IPL_ABI::coverPaintDecision(image,enabled,flag,counter,threshold,inhibit,requested);return (p.active?1:0)|(p.drawImage?2:0)|(p.requestCapture?4:0); }\nextern "C" int32_t advance(int32_t counter){return IPL_ABI::coverPaintNextCounter(counter); }\n')
    subprocess.run(['g++','-std=c++11','-shared','-fPIC',str(source),'-o',str(root/'helper.so')],check=True)
    lib=ctypes.CDLL(str(root/'helper.so'))
    lib.finished.argtypes=[ctypes.c_bool]*3+[ctypes.c_int32]*2
    lib.finished.restype=ctypes.c_bool
    lib.changed.argtypes=[ctypes.c_int32]*3+[ctypes.c_bool]*2+[ctypes.c_int32]
    lib.changed.restype=ctypes.c_int32
    values=[-2147483648,-1,0,1,2,3,2147483647]
    comparisons=0
    for image,enabled,state,count,threshold in product((0,1),(0,1),(0,1),values,values):
        word(0x9c,image);mu.mem_write(obj+0x70,bytes([enabled]));mu.mem_write(obj+0xa0,bytes([state]));word(0xa4,count);word(0x1c,threshold)
        actual=invoke(0x18d9c,obj)
        assert actual==lib.finished(image,enabled,state,count,threshold),(image,enabled,state,count,threshold,actual)
        comparisons+=1
    for same,event,image,enabled in product((0,1),(-1,0,1,2),(0,1),(0,1)):
        surface=7 if same else 8
        word(0x3c,7);word(0x9c,image);mu.mem_write(obj+0x70,bytes([enabled]));word(0xa4,9)
        invoke(0x17d14,obj,surface,event,0)
        actual=struct.unpack('<i',mu.mem_read(obj+0xa4,4))[0]
        assert actual==lib.changed(7,surface,event,image,enabled,9),(same,event,image,enabled,actual)
        comparisons+=1
    print(f'PASS: {comparisons} original Thumb/helper comparisons; I_GetAlcPtr mocked null')

# Test original TopRenderCoverCallback with only external canvas/capture effects mocked.
canvas,canvasTable,device,deviceTable=0x101000,0x102000,0x103000,0x104000
drawStub,captureStub=0x301000,0x302000
mu.mem_write(canvas,struct.pack('<I',canvasTable))
mu.mem_write(canvasTable+0xcc,struct.pack('<I',drawStub|1))
mu.mem_write(device,struct.pack('<I',deviceTable))
mu.mem_write(deviceTable+0x128,struct.pack('<I',captureStub|1))
effects=[]
capture_counter_override=None
draw_counter_override=None
def effect_hook(emu,address,size,data):
    if address in (drawStub,captureStub):
        effects.append('draw' if address==drawStub else 'capture')
        if address==drawStub and draw_counter_override is not None:word(0xa4,draw_counter_override)
        if address==captureStub:
            assert emu.reg_read(UC_ARM_REG_R3)==obj+4,'wrong business observer subobject'
            assert mu.mem_read(obj+0x98,1)[0]==1,'request flag must be set before callback'
            if capture_counter_override is not None:word(0xa4,capture_counter_override)
        emu.reg_write(UC_ARM_REG_PC,emu.reg_read(UC_ARM_REG_LR))
mu.hook_add(UC_HOOK_CODE,effect_hook)
lib.paint.argtypes=[ctypes.c_bool]*3+[ctypes.c_int32]*2+[ctypes.c_bool]*2
lib.paint.restype=ctypes.c_int
lib.advance.argtypes=[ctypes.c_int32]
lib.advance.restype=ctypes.c_int32
paint_cases=0
for image,enabled,flag,count,threshold,inhibit,requested in product((0,1),(0,1),(0,1),[-2147483648,-1,0,2,3,9,10,11,2147483647],[2,10],(0,1),(0,1)):
    word(0x9c,image);word(0x10,device);word(0xa4,count);word(0x1c,threshold)
    for offset,value in ((0x70,enabled),(0xa0,flag),(0x15,inhibit),(0x98,requested)):
        mu.mem_write(obj+offset,bytes([value]))
    effects.clear()
    decision=lib.paint(image,enabled,flag,count,threshold,inhibit,requested)
    invoke(0x18898,obj,canvas)
    expected_effects=(['draw'] if decision&2 else [])+(['capture'] if decision&4 else [])
    assert effects==expected_effects,(image,enabled,flag,count,threshold,inhibit,requested,effects,expected_effects)
    actual=struct.unpack('<i',mu.mem_read(obj+0xa4,4))[0]
    assert actual==(lib.advance(count) if decision&1 else count),(count,actual)
    assert mu.mem_read(obj+0x98,1)[0]==(1 if decision&4 else requested)
    paint_cases+=1
print(f'PASS: {paint_cases} original paint/helper comparisons; draw/capture effects mocked')

# Original reloads the counter after the external capture method returns.
capture_counter_override=201
word(0x9c,1);word(0x10,device);word(0xa4,10);word(0x1c,2)
for offset,value in ((0x70,1),(0xa0,1),(0x15,0),(0x98,0)):mu.mem_write(obj+offset,bytes([value]))
effects.clear()
invoke(0x18898,obj,canvas)
assert effects==['capture']
assert struct.unpack('<i',mu.mem_read(obj+0xa4,4))[0]==lib.advance(201)==202
print('PASS: capture callback counter mutation is reloaded before increment')

capture_counter_override=None
draw_counter_override=10
word(0xa4,2);mu.mem_write(obj+0x98,b'\x00');effects.clear()
invoke(0x18898,obj,canvas)
assert effects==['draw','capture']
assert lib.paint(1,1,1,2,2,0,0)&2
assert lib.paint(1,1,1,10,2,0,0)&4
assert struct.unpack('<i',mu.mem_read(obj+0xa4,4))[0]==11
print('PASS: draw callback counter mutation is reloaded before capture decision')
