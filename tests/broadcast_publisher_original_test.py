"""Compare original/rebuilt Publisher resolver and list state callbacks."""
import argparse
from pathlib import Path
import struct
import subprocess
import sys
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[1];sys.path.insert(0,str(root/'tools'))
from inspect_packet_factory import ELF32,plt_symbol
p=argparse.ArgumentParser();p.add_argument('--so',type=Path,required=True);p.add_argument('--nm',required=True);args=p.parse_args()
symbols={line.split()[2]:int(line.split()[0],16) for line in subprocess.check_output([args.nm,'-n',str(args.so)],text=True).splitlines() if len(line.split())==3 and line.split()[1] not in ('U','w','v')}
original=ELF32(root/'reference/v21/libInstrumentPannelLarge.so')
# 0x45584 is the interworking veneer used by resolver 0x15258.
target=(0x45594+original.u32(0x45590))&0xffffffff
publisher_symbol='_ZN3GPI9Broadcast8internal9Publisher3getEPvPFS3_vEPFvS3_E'
assert plt_symbol(original,target)[1]==publisher_symbol

def run(rebuilt,count):
 elf=ELF32(args.so) if rebuilt else original
 m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x400000)
 for kind,off,addr,_,size,*_ in elf.segments:
  if kind==1:m.mem_write(addr,elf.data[off:off+size])
 def put(a,v):m.mem_write(a,struct.pack('<I',v))
 def word(a):return struct.unpack('<I',m.mem_read(a,4))[0]
 handlers={0x1c0000:'publisherKey',0x1c0004:'destroyWrapper'}
 cursor=0x1d0000
 for got,(_,symbol) in elf.relocation_symbols('.rel.plt').items():
  if rebuilt and symbol in symbols:value=symbols[symbol]|1
  else:handlers[cursor]=symbol;value=cursor|1;cursor+=4
  put(got,value)
 broadcaster,table,key,cached=0x90000,0x91000,0x92000,0x93000
 put(broadcaster,table);put(table,0x1c0001)
 callbacks=[];trace=[];live={};heap=0x200000
 def hook(e,a,size,_):
  nonlocal heap
  if a not in handlers:return
  name=handlers[a];r0=e.reg_read(UC_ARM_REG_R0);r1=e.reg_read(UC_ARM_REG_R1);r2=e.reg_read(UC_ARM_REG_R2)
  if name=='publisherKey':assert r0==broadcaster;value=key;trace.append('key')
  elif name==publisher_symbol:
   assert r0==key and r1&1 and r2&1;callbacks.extend([r1,r2]);value=cached;trace.append('get')
  elif name=='_Znwj':
   assert r0==16;value=heap;heap+=0x100;live[value]=16;m.mem_write(value,bytes([0xcc])*16);trace.append('allocate-state')
  elif name in ('_ZNSt6__ndk15mutexD1Ev','_ZNSt6__ndk15mutexD2Ev'):
   assert r0==0x20000c;value=0;trace.append('destroy-mutex')
  elif name=='destroyWrapper':
   assert r0 in live;del live[r0];value=0;trace.append(('wrapper',r0))
  elif name=='_ZdlPv':
   assert r0 in live;del live[r0];value=0;trace.append(('delete',r0))
  else:raise AssertionError(name)
  e.reg_write(UC_ARM_REG_R0,value);e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
 m.hook_add(UC_HOOK_CODE,hook)
 def execute(entry,r0):
  m.reg_write(UC_ARM_REG_R0,r0);m.reg_write(UC_ARM_REG_SP,0x180000);m.reg_write(UC_ARM_REG_LR,0x1f0001)
  m.emu_start(entry|1,0x1f0000,count=3000)
  assert m.reg_read(UC_ARM_REG_PC)==0x1f0000
  return m.reg_read(UC_ARM_REG_R0)
 entry=next(a for n,a in symbols.items() if 'resolveBroadcastList' in n) if rebuilt else 0x15258
 assert execute(entry,broadcaster)==cached and trace==['key','get']
 state=execute(callbacks[0],0)
 assert state==0x200000 and bytes(m.mem_read(state,16))==struct.pack('<4I',state,state,0,0)
 # Supply identical populated nodes with inert shared state and tracked wrappers.
 wrapper_table=0xa0000;put(wrapper_table+4,0x1c0005)
 nodes=[0x210000+i*0x100 for i in range(count)]
 for i,node in enumerate(nodes):
  live[node]=32;wrapper=node+0x40;live[wrapper]=16;put(wrapper,wrapper_table)
  m.mem_write(node,bytes(32));put(node,nodes[i+1] if i+1<count else state);put(node+4,nodes[i-1] if i else state)
  put(node+12,wrapper)
 if nodes:put(state,nodes[0]);put(state+4,nodes[-1]);put(state+8,count)
 execute(callbacks[1],state)
 assert not live
 before=len(trace);execute(callbacks[1],0);assert len(trace)==before
 expected=['key','get','allocate-state','destroy-mutex']
 for node in reversed(nodes):expected.extend([('wrapper',node+0x40),('delete',node)])
 expected.append(('delete',state))
 assert trace==expected,(trace,expected)
 return trace
for count in (0,1,3):assert run(False,count)==run(True,count)
print('Original/rebuilt Publisher key/get ABI and create/destroy callbacks (empty/1/3 nodes, null disposal): PASS')
