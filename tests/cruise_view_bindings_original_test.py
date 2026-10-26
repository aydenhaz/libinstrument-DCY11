"""Compare original lifecycle slices and linked ARM host bindings.
External APIs are mocked; actual pointer arithmetic, stores and calls execute.
"""
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
parser=argparse.ArgumentParser();parser.add_argument('--so',type=Path,required=True);parser.add_argument('--nm',required=True);args=parser.parse_args()
symbols={line.split()[2]:int(line.split()[0],16) for line in subprocess.check_output([args.nm,'-n',str(args.so)],text=True).splitlines() if len(line.split())==3 and line.split()[1] not in ('U','w','v')}
F,P,G,S,M,A,C,N,X=0x90000,0x91000,0x92000,0x93000,0x94000,0x95000,0x96000,0x97000,0x98000
SP=0x180000
names={
 '_ZN9GNS_FRAME10CGFragment12onCreateViewEPNS_11CGViewGroupE':'baseCreate',
 '_ZN9GNS_FRAME10CGFragment13onDestroyViewEv':'baseDestroy',
 '_ZN3hsl3HSL21GetSystemNotifyHandleEv':'system',
 '_ZN3hsl12SystemNotify18AddTestNumObserverEPNS_15TestNumObserverE':'addTest',
 '_ZN3hsl12SystemNotify21RemoveTestNumObserverEPNS_15TestNumObserverE':'removeTest',
 '_ZN3hsl3HSL14GetGuideHandleEv':'guide',
 '_ZN3hsl3HSL12GetPosHandleEv':'pos',
 '_ZN3hsl3HSL15GetConfigHandleEv':'config',
 '_ZN3hsl6Config17AddConfigObserverEPNS_14ConfigObserverE':'addConfig',
 '_ZN3hsl6Config20RemoveConfigObserverEPNS_14ConfigObserverE':'removeConfig',
 '_ZN3hsl3HSL16GetMapViewHandleEN2bl11MapEngineIDE':'map',
 'I_GetAlcPtr':'logging',
}
def run(path, rebuilt, hasmap, hasservice, haspos, hasconfig, full=False):
 elf=ELF32(path); m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x200000)
 for kind,offset,address,_,size,*_ in elf.segments:
  if kind==1:m.mem_write(address,elf.data[offset:offset+size])
 handlers={}; next_stub=0x1c0000
 for got,(_,symbol) in elf.relocation_symbols('.rel.plt').items():
  if rebuilt and symbol in symbols and symbol not in names:
   target=symbols[symbol]|1
  else:
   target=next_stub|1;handlers[next_stub]=names.get(symbol,'UNEXPECTED '+symbol);next_stub+=4
  m.mem_write(got,struct.pack('<I',target))
 def word(address,value):m.mem_write(address,struct.pack('<I',value))
 for obj in (G,S,M,P):word(obj,obj+0x8000)
 for obj,offset,name in ((G,0x20,'service'),(S,0x18,'addGuide'),(S,0x1c,'removeGuide'),(M,0x2b4,'mapAux'),(P,0x1c,'addPos'),(P,0x20,'removePos')):
  handlers[next_stub]=name;word(obj+0x8000+offset,next_stub|1);next_stub+=4
 m.mem_write(F,bytes([0xa5])*0x3dc);word(F+0x3c0,M if hasmap else 0)
 trace=[]
 if full:
  location=next(address for name,address in symbols.items() if '24initializeCruiseLocation' in name)
  handlers[location & ~1]='location'
  for needle,label in (('attachCruiseBroadcasts','broadcast-add'),('detachCruiseBroadcasts','broadcast-remove')):
   address=next(address for name,address in symbols.items() if needle in name)
   handlers[address & ~1]=label
 def hook(e,address,size,_):
  if address not in handlers:return
  name=handlers[address];r0=e.reg_read(UC_ARM_REG_R0);r1=e.reg_read(UC_ARM_REG_R1)
  if name=='logging':value=0
  elif name in ('system','guide','pos','config'):
   value={'system':N,'guide':G,'pos':P if haspos else 0,'config':C if hasconfig else 0}[name];trace.append(name)
  elif name in ('broadcast-add','broadcast-remove'):
   assert r0==F;trace.append(name);value=0
  elif name=='map':
   assert r0==7;value=M if hasmap else 0;trace.append(name)
  elif name=='location':
   assert r0==(M if hasmap else 0);value=0;trace.append(name)
  elif name=='service':assert r0==G;value=S if hasservice else 0;trace.append(name)
  elif name=='mapAux':assert r0==M;value=A;trace.append(name)
  elif name=='baseCreate':
   assert r0==F and r1==X;value=0;trace.append(name)
  elif name=='baseDestroy':assert r0==F;value=0;trace.append(name)
  elif name in ('addTest','removeTest','addGuide','removeGuide','addPos','removePos','addConfig','removeConfig'):
   receiver,offset={'addTest':(N,0x2e4),'removeTest':(N,0x2e4),'addGuide':(S,0x364),'removeGuide':(S,0x364),'addPos':(P,0x368),'removePos':(P,0x368),'addConfig':(C,0x38c),'removeConfig':(C,0x38c)}[name]
   assert (r0,r1)==(receiver,F+offset),(name,hex(r0),hex(r1));trace.append(name);value=0
   if name=='addTest':word(F+0x308,7)
  else:raise AssertionError(name)
  e.reg_write(UC_ARM_REG_R0,value);e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
 m.hook_add(UC_HOOK_CODE,hook)
 def execute(start,end):
  m.reg_write(UC_ARM_REG_SP,SP);m.reg_write(UC_ARM_REG_LR,0x1f0001)
  m.reg_write(UC_ARM_REG_R0,F);m.reg_write(UC_ARM_REG_R1,X);m.reg_write(UC_ARM_REG_R4,F);m.reg_write(UC_ARM_REG_R5,X)
  m.emu_start(start|1,end,count=1500);assert m.reg_read(UC_ARM_REG_PC)==end,hex(m.reg_read(UC_ARM_REG_PC))
 if full:
  for method in ('12onCreateView','13onDestroyView'):
   symbol=next(name for name in symbols if '18AutoCruiseFragment'+method in name)
   execute(symbols[symbol],0x1f0000)
 elif rebuilt:
  for symbol in ('_ZN7IPL_ABI15beginCruiseViewEPvS0_','_ZN7IPL_ABI17attachCruiseGuideEPv','_ZN7IPL_ABI20attachCruiseViewTailEPv','_ZN7IPL_ABI16detachCruiseViewEPv'):
   execute(symbols[symbol],0x1f0000)
 else:
  for start,end in ((0x3ebac,0x3ebc0),(0x3ebcc,0x3ec22),(0x3eed4,0x3ef10),(0x3f482,0x3f4c4),(0x3f4fc,0x3f502)):
   execute(start,end)
 fields=bytes(m.mem_read(F,0x3dc))
 expected=bytearray([0xa5]*0x3dc)
 for offset,value in ((0x308,7),(0x3c0,M if hasmap else 0),(0x3c8,P if haspos else 0),(0x3d0,C if hasconfig else 0)):
  struct.pack_into('<I',expected,offset,value)
 if hasmap:struct.pack_into('<I',expected,0x3c4,A)
 assert fields==expected,'unexpected fragment write'
 return trace,fields
for hasmap in (0,1):
 for hasservice in (0,1):
  for haspos in (0,1):
   for hasconfig in (0,1):
    flags=(hasmap,hasservice,haspos,hasconfig)
    original=run(root/'reference/v21/libInstrumentPannelLarge.so',False,*flags)
    assert original==run(args.so,True,*flags)
    trace,fields=run(args.so,True,*flags,full=True)
    assert fields==original[1]
    expected=list(original[0]);expected.insert(3,'map')
    index=expected.index('addGuide')+1 if hasservice else expected.index('service')+1
    expected.insert(index,'location')
    index=expected.index('removeTest')-1
    expected[index:index]=['broadcast-add','broadcast-remove']
    assert trace==expected,(trace,expected)
print('Original vs rebuilt ARM view bindings: 16 cases, ordering, offsets, cache writes and paired removal PASS')
