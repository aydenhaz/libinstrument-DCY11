#!/usr/bin/env python3
"""Original V21 RGB accumulation/first conversion vs recovered C++ valid-image helper.
Requires dependencies of cover_original_differential_test.py. Graphics services and
remaining string/configuration side effects are not tested.
"""
import ctypes
from pathlib import Path
import random
import runpy
import struct
import subprocess
import tempfile
from unicorn.arm_const import UC_ARM_REG_C1_C0_2, UC_ARM_REG_FPEXC, UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_S0, UC_ARM_REG_S16, UC_ARM_REG_S18, UC_ARM_REG_S20
context=runpy.run_path(str(Path(__file__).with_name('cover_original_differential_test.py')))
mu,obj,stack=context['mu'],context['obj'],context['stack']
word=context['word']
mu.reg_write(UC_ARM_REG_C1_C0_2,mu.reg_read(UC_ARM_REG_C1_C0_2)|0xf00000)
mu.reg_write(UC_ARM_REG_FPEXC,1<<30)
data_address=0x105000
header=Path(__file__).resolve().parents[1]/'internal/abi/CoverRgbAverage.h'
randomizer=random.Random(21)
with tempfile.TemporaryDirectory() as directory:
    root=Path(directory)
    (root/'helper.cpp').write_text('#include "'+str(header)+'"\nextern "C" bool average(const uint8_t* p,size_t n,int w,int h,int s,IPL_ABI::CoverRgbAverage* out){return IPL_ABI::coverRgbAverage(p,n,w,h,s,*out);}\n')
    subprocess.run(['g++','-shared','-fPIC','-std=c++11',str(root/'helper.cpp'),'-o',str(root/'helper.so')],check=True)
    lib=ctypes.CDLL(str(root/'helper.so'))
    lib.average.argtypes=[ctypes.c_void_p,ctypes.c_size_t,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_void_p]
    lib.average.restype=ctypes.c_bool
    cases=0
    for width,height in [(1,1),(2,1),(3,5),(16,16),(31,17)]:
        for stride in (3,4):
            for pattern in ('zero','white','random'):
                size=width*height*stride
                data=bytes(size) if pattern=='zero' else bytes([255])*size if pattern=='white' else bytes(randomizer.randrange(256) for _ in range(size))
                mu.mem_write(data_address,data)
                word(0x80,width);word(0x84,height);word(0x88,data_address);word(0x8c,size)
                mu.mem_write(obj+0x7c,bytes([stride]));mu.mem_write(obj+0x98,b'\x01');mu.mem_write(obj+0xa0,b'\x00')
                mu.reg_write(UC_ARM_REG_R0,obj);mu.reg_write(UC_ARM_REG_R1,23)
                mu.reg_write(UC_ARM_REG_SP,stack+0x8000);mu.reg_write(UC_ARM_REG_LR,0x300001)
                # Stop before first external integer/string formatter. The red
                # channel has already passed original VCVT.S32.F32.
                mu.emu_start(0x17e9d,0x17f2e,count=20000)
                first=ctypes.c_int32(mu.reg_read(UC_ARM_REG_S0)).value
                as_float=lambda reg:struct.unpack('<f',struct.pack('<I',mu.reg_read(reg)))[0]
                divisor=as_float(UC_ARM_REG_S20)
                green=int(ctypes.c_float(as_float(UC_ARM_REG_S18)/divisor).value)
                blue=int(ctypes.c_float(as_float(UC_ARM_REG_S16)/divisor).value)
                result=(ctypes.c_int32*3)();buffer=ctypes.create_string_buffer(data)
                assert lib.average(buffer,size,width,height,stride,result)
                assert list(result)==[first,green,blue],(width,height,stride,pattern,list(result),first,green,blue)
                assert mu.mem_read(obj+0x98,1)==b'\x00' and mu.mem_read(obj+0xa0,1)==b'\x01'
                cases+=1
    print(f'PASS: {cases} original RGB/RGBA accumulation and state comparisons; stopped before formatter')
