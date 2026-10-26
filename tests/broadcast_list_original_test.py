"""Run original list operations with mocked Callable/runtime operations.
List behavior is tested; rebuilt shared-control ABI interop remains separate.
"""
from pathlib import Path
import struct
import sys
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[1];sys.path.insert(0,str(root/'tools'))
from inspect_packet_factory import ELF32
elf=ELF32(root/'reference/v21/libInstrumentPannelLarge.so')
m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x400000)
for kind,off,addr,_,size,*_ in elf.segments:
 if kind==1:m.mem_write(addr,elf.data[off:off+size])
def u32(a):return struct.unpack('<I',m.mem_read(a,4))[0]
def put(a,v):m.mem_write(a,struct.pack('<I',v))
state,broadcaster,source=0x90000,0x91000,0x92000
wrapper_table,method_table=0x93000,0x94000
put(state,state);put(state+4,state);put(state+8,0)
put(wrapper_table+4,0x1c0001);put(wrapper_table+12,0x1c0005);put(method_table,0x1c0009)
handlers={0x1c0000:'destroyWrapper',0x1c0004:'cloneWrapper',0x1c0008:'equals',0x15258:'resolve'}
address=0x1d0000
for got,(_,name) in elf.relocation_symbols('.rel.plt').items():
 handlers[address]=name;put(got,address|1);address+=4
heap=0x200000;allocations={};locked=False;events=[]
def allocate(size):
 global heap
 result=heap;heap+=(size+15)&~15;allocations[result]=size;m.mem_write(result,bytes(size));return result
def free(pointer):
 assert pointer in allocations,hex(pointer)
 del allocations[pointer]
def hook(e,address,size,_):
 global locked
 if address not in handlers:return
 name=handlers[address];a=e.reg_read(UC_ARM_REG_R0);b=e.reg_read(UC_ARM_REG_R1)
 if name=='resolve':assert a==broadcaster;value=state
 elif name=='_Znwj':assert locked and a==32;value=allocate(a);events.append(('node-new',locked))
 elif name=='_ZdlPv':assert locked;free(a);events.append(('node-delete',locked));value=0
 elif name=='cloneWrapper':
  value=allocate(16);m.mem_write(value,bytes(m.mem_read(a,16)));events.append(('clone',locked))
 elif name=='destroyWrapper':free(a);events.append(('wrapper-delete',locked));value=0
 elif name=='equals':value=int(b!=0 and u32(a+4)==u32(b+4))
 elif name=='_ZNSt6__ndk15mutex4lockEv':assert a==state+12 and not locked;locked=True;events.append(('lock',));value=0
 elif name=='_ZNSt6__ndk15mutex6unlockEv':assert a==state+12 and locked;locked=False;events.append(('unlock',));value=0
 else:raise AssertionError(name)
 e.reg_write(UC_ARM_REG_R0,value);e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
m.hook_add(UC_HOOK_CODE,hook)
expected=[]
operations=[('add',16,1),('add',16,1),('add',17,1),('add',16,2),('add',16,3),
 ('remove',99,1),('remove',16,1),('remove',16,1),('remove',17,1),('remove',16,2),('remove',16,3)]
for action,channel,key in operations:
 wrapper=allocate(16);put(wrapper,wrapper_table)
 method=0xa0000+key*16;put(method,method_table);put(method+4,key)
 put(source,0x68f5c);put(source+4,wrapper);put(source+8,0);put(source+12,method);put(source+16,0)
 put(broadcaster+4,channel)
 before=len(events)
 m.reg_write(UC_ARM_REG_R0,broadcaster);m.reg_write(UC_ARM_REG_R1,source)
 m.reg_write(UC_ARM_REG_SP,0x180000);m.reg_write(UC_ARM_REG_LR,0x1f0001)
 m.emu_start((0x15084 if action=='add' else 0x166dc)|1,0x1f0000,count=5000)
 assert m.reg_read(UC_ARM_REG_PC)==0x1f0000 and not locked
 pair=(channel,key)
 if action=='add':
  if pair not in expected:expected.insert(0,pair)
 elif pair in expected:
  expected.remove(pair)
  assert events[-1]==('wrapper-delete',False),events[before:]
 nodes=[];prev=state;node=u32(state)
 while node!=state:
  assert u32(node+4)==prev
  nodes.append((u32(node+28),u32(u32(node+20)+4)))
  prev=node;node=u32(node)
 assert u32(state+4)==prev and nodes==expected and u32(state+8)==len(expected)
 free(wrapper)
 assert len(allocations)==len(expected)*2
assert not allocations and not expected
print('Original broadcast list: 11 operations, channel/identity matching, insertion order, balanced copies and post-unlock final release PASS')
