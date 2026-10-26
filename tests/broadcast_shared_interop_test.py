"""Execute original Callable copy/equality/destruction against rebuilt objects.
Uses real NDK libc++ shared-control and RTTI code; only OS/heap primitives mocked.
"""
import argparse
from pathlib import Path
import struct
import sys
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_HOOK_CODE
from unicorn.arm_const import *
root=Path(__file__).resolve().parents[1];sys.path.insert(0,str(root/'tools'))
from inspect_packet_factory import ELF32
p=argparse.ArgumentParser();p.add_argument('--so',type=Path,required=True);p.add_argument('--runtime',type=Path,required=True);args=p.parse_args()
m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x4000000)
images=[(ELF32(root/'reference/v21/libInstrumentPannelLarge.so'),0),(ELF32(args.so),0x200000),(ELF32(args.runtime),0x800000)]
def symbols(e):
 sec=e.sections['.dynsym'];names=e.dynsym_names()
 return [(names[i],*struct.unpack_from('<IIIBBH',e.data,offset)[1:]) for i,offset in enumerate(range(sec[4],sec[4]+sec[5],16))]
def word(a):return struct.unpack('<I',m.mem_read(a,4))[0]
def put(a,v):m.mem_write(a,struct.pack('<I',v&0xffffffff))
definitions={};all_symbols=[]
for e,base in images:
 for kind,off,addr,_,size,*_ in e.segments:
  if kind==1:m.mem_write(base+addr,e.data[off:off+size])
 syms=symbols(e);all_symbols.append(syms)
 for name,value,size,info,other,index in syms:
  if index and name:definitions.setdefault(name,base+value)
# Prefer the rebuilt specialization and runtime definitions to original exports.
for (e,base),syms in zip(images[1:],all_symbols[1:]):
 for name,value,size,info,other,index in syms:
  if index and name:definitions[name]=base+value
stubs={};stub_address=0x1c00000
primitive_names={'_Znwj','_Znaj','_ZdlPv','_ZdaPv','_ZdlPvj','malloc','free','calloc','memcpy','memmove','memset','memcmp','strcmp','strlen','abort','__stack_chk_fail'}
def stub(name):
 global stub_address
 for address,existing in stubs.items():
  if existing==name:return address|1
 result=stub_address;stub_address+=4;stubs[result]=name;return result|1
put(0x1f00000,0xa55a1234);definitions['__stack_chk_guard']=0x1f00000
for (e,base),syms in zip(images,all_symbols):
 for section in ('.rel.dyn','.rel.plt'):
  if section not in e.sections:continue
  sec=e.sections[section]
  for address,info in struct.iter_unpack('<II',e.data[sec[4]:sec[4]+sec[5]]):
   typ=info&255;index=info>>8;address+=base
   if typ==23:put(address,word(address)+base);continue
   name,value,size,binding,other,shndx=syms[index]
   resolved=stub(name) if name in primitive_names else definitions.get(name)
   if resolved is None:resolved=stub(name)
   if typ==2:put(address,word(address)+resolved)
   elif typ in (21,22):put(address,resolved)
   else:raise AssertionError(('relocation',typ,name))
heap=0x2000000;live={};freed=[];calls=[];states={};broadcasts={};locks=set();events=[]
def alloc(size):
 global heap
 address=heap;heap+=(size+15)&~15;live[address]=size;m.mem_write(address,bytes(size));return address
def string(address):
 data=bytearray()
 while m.mem_read(address,1)!=b'\0':data.extend(m.mem_read(address,1));address+=1
 return bytes(data)
def hook(e,address,size,_):
 if address not in stubs:return
 name=stubs[address];a=e.reg_read(UC_ARM_REG_R0);b=e.reg_read(UC_ARM_REG_R1);c=e.reg_read(UC_ARM_REG_R2)
 if name in ('_Znwj','_Znaj','malloc'):result=alloc(a)
 elif name=='calloc':result=alloc(a*b)
 elif name in ('_ZdlPv','_ZdaPv','_ZdlPvj','free'):
  if a:assert a in live,('invalid free',hex(a));freed.append(a);del live[a]
  result=0
 elif name in ('memcpy','memmove'):m.mem_write(a,bytes(m.mem_read(b,c)));result=a
 elif name=='memset':m.mem_write(a,bytes([b&255])*c);result=a
 elif name in ('memcmp','strcmp'):
  x,y=(bytes(m.mem_read(a,c)),bytes(m.mem_read(b,c))) if name=='memcmp' else (string(a),string(b));result=(x>y)-(x<y)
 elif name=='strlen':result=len(string(a))
 elif name=='messageTarget':calls.append((a,b));result=0
 elif name=='_ZN9GNS_FRAME13CGBroadcaster11getInstanceEj':
  assert a in broadcasts;events.append(('get',a));result=broadcasts[a]
 elif name=='publisherKey':result=a
 elif name=='_ZN3GPI9Broadcast8internal9Publisher3getEPvPFS3_vEPFvS3_E':
  assert a in states and b and c;result=states[a]
 elif name=='pthread_mutex_lock':
  assert a not in locks;locks.add(a);result=0
 elif name=='pthread_mutex_unlock':
  assert a in locks;locks.remove(a);result=0
 elif name=='pthread_mutex_destroy':assert a not in locks;result=0
 else:raise AssertionError(('unexpected external',name,hex(address)))
 e.reg_write(UC_ARM_REG_R0,result&0xffffffff);e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
m.hook_add(UC_HOOK_CODE,hook)
m.reg_write(UC_ARM_REG_C1_C0_2,0xf00000);m.reg_write(UC_ARM_REG_FPEXC,0x40000000)
def execute(address,*values):
 for reg,value in zip((UC_ARM_REG_R0,UC_ARM_REG_R1,UC_ARM_REG_R2,UC_ARM_REG_R3),values):m.reg_write(reg,value)
 m.reg_write(UC_ARM_REG_SP,0x3000000);m.reg_write(UC_ARM_REG_LR,0x3100001)
 try:m.emu_start(address,0x3100000,count=100000)
 except Exception:
  print('PC',hex(m.reg_read(UC_ARM_REG_PC)));raise
 assert m.reg_read(UC_ARM_REG_PC)==0x3100000,hex(m.reg_read(UC_ARM_REG_PC))
 return m.reg_read(UC_ARM_REG_R0)
source,destination,peer,different,target,table,message=0x1000000,0x1000100,0x1000200,0x1000300,0x1100000,0x1101000,0x1102000
put(target,table);put(table+0xe4,stub('messageTarget'));put(table+0xec,stub('messageTarget'))
factory=definitions['_ZN7IPL_ABI27makeCruiseBroadcastCallableEPvNS_19CruiseBroadcastSlotE']
execute(factory,source,target,0);execute(factory,peer,target,0);execute(factory,different,target,1)
assert len(live)==6,live
put(destination,0x68f5c)
execute(0x152b3,destination,source) # original copy clones rebuilt wrapper
assert len(live)==7 and word(destination+4)!=word(source+4)
assert word(destination+12)==word(source+12) and word(destination+16)==word(source+16)
assert execute(0x151ff,destination,peer)==1 # original equals -> real rebuilt RTTI
assert execute(0x151ff,destination,different)==0
execute(0x15135,source) # original release, while destination retains shared method
wrapper=word(destination+4);execute(word(word(wrapper)+8),wrapper,message)
assert calls==[(target,message)]
for obj in (destination,peer,different):execute(0x15135,obj)
assert not live,live
assert len(freed)==7
print('Original copy/equality/destruction + rebuilt wrapper/method + real NDK libc++ RTTI/shared release: PASS (7 allocations freed once)')

# Execute actual rebuilt attach/detach with the two host broadcaster handles.
# Publisher lookup and OS mutex calls are mocked; list/Callable/runtime are real.
state_factory=definitions['_ZN7IPL_ABI24createBroadcastListStateEv']
state_destroy=definitions['_ZN7IPL_ABI25destroyBroadcastListStateEPNS_18BroadcastListStateE']
for i,channel in enumerate((0x10,0x10004)):
 broadcaster=0x1200000+i*0x100
 vtable=broadcaster+0x40
 put(broadcaster,vtable);put(vtable,stub('publisherKey'));put(broadcaster+4,channel)
 broadcasts[channel]=broadcaster;states[broadcaster]=execute(state_factory)
attach=definitions['_ZN7IPL_ABI22attachCruiseBroadcastsEPv']
detach=definitions['_ZN7IPL_ABI22detachCruiseBroadcastsEPv']
execute(attach,target);execute(attach,target)
assert events==[('get',0x10),('get',0x10004)]*2
for state in states.values():
 assert word(state+8)==1
 node=word(state);wrapper=word(node+12)
 execute(word(word(wrapper)+8),wrapper,message)
assert calls==[(target,message)]*3
execute(detach,target);execute(detach,target)
assert not locks and all(word(state+8)==0 for state in states.values())
for state in states.values():execute(state_destroy,state)
assert not live,live
print('Rebuilt attach/duplicate/message dispatch/detach/repeated detach with real list and libc++ ownership: PASS')
