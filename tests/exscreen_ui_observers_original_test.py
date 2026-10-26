"""Execute original and rebuilt ARM UI-observer operations with host mocks."""
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
p=argparse.ArgumentParser();p.add_argument('--so',type=Path,required=True);p.add_argument('--nm',required=True);args=p.parse_args()
symbols={line.split()[2]:int(line.split()[0],16) for line in subprocess.check_output([args.nm,'-n',str(args.so)],text=True).splitlines() if len(line.split())==3 and line.split()[1] not in ('U','w','v')}
O,LT,AT=0x90000,0x96000,0x97000
OLD,H1,H2=0x110000,0x120000,0x130000
HEAP_SIZE=8192
original={'addMapRenderObserver':0x2d55c,'removeMapRenderObserver':0x2d6f8,
 'addUiUpdateObserver':0x2d7b0,'removeUiUpdateObserver':0x2d964,
 'setProjectionSizeListener':0x2deac,'setUiStyleListener':0x2df28}
def run(rebuilt, offset, flags, capacity, count, operations):
 path=args.so if rebuilt else root/'reference/v21/libInstrumentPannelLarge.so'
 elf=ELF32(path);m=Uc(UC_ARCH_ARM,UC_MODE_THUMB);m.mem_map(0,0x200000)
 for kind,off,addr,_,size,*_ in elf.segments:
  if kind==1:m.mem_write(addr,elf.data[off:off+size])
 def word(a,v):m.mem_write(a,struct.pack('<I',v))
 handlers={0x99000:'lock',0x99004:'unlock',0x99008:'allocate',0x9900c:'release'}
 cursor=0x1c0000
 for got,(_,name) in elf.relocation_symbols('.rel.plt').items():
  if rebuilt and name in symbols:target=symbols[name]|1
  else:
   handlers[cursor]='logging' if name=='I_GetAlcPtr' else name;target=cursor|1;cursor+=4
  word(got,target)
 m.mem_write(O,bytes([0xa5])*0x150)
 word(LT+8,0x99001);word(LT+12,0x99005)
 word(AT+8,0x99009);word(AT+12,0x9900d)
 word(O+offset,LT)
 vector=O+offset+0x10
 if offset!=0xb4:
  word(vector,OLD if capacity else 0);word(vector+4,capacity);word(vector+8,count);word(vector+12,AT)
  m.mem_write(vector+16,bytes([flags]))
 for address in (OLD,H1,H2):m.mem_write(address,bytes([0xcc])*HEAP_SIZE)
 for i in range(count):word(OLD+i*4,0xa0000+i*4)
 trace=[];locked=False;live=OLD if capacity else 0;pending=0
 def hook(e,address,size,_):
  nonlocal locked,live,pending
  if address not in handlers:return
  name=handlers[address];r0=e.reg_read(UC_ARM_REG_R0);r1=e.reg_read(UC_ARM_REG_R1)
  if name=='logging':value=0
  elif name=='lock':
   assert not locked and r0==O+offset;locked=True;trace.append(('lock',));value=0
  elif name=='unlock':
   assert locked and r0==O+offset;locked=False;trace.append(('unlock',));value=0
  elif name=='allocate':
   assert locked and r0==vector+12 and 0<r1<=HEAP_SIZE
   pending=H1 if live!=H1 else H2;m.mem_write(pending,bytes([0xcc])*HEAP_SIZE)
   trace.append(('allocate',r1));value=pending
  elif name=='release':
   assert locked and r0==vector+12 and r1==live
   trace.append(('release',r1));live=pending;pending=0;value=0
  else:raise AssertionError(name)
  e.reg_write(UC_ARM_REG_R0,value);e.reg_write(UC_ARM_REG_PC,e.reg_read(UC_ARM_REG_LR))
 m.hook_add(UC_HOOK_CODE,hook)
 results=[]
 for name,observer in operations:
  target=next(value for symbol,value in symbols.items() if name in symbol) if rebuilt else original[name]
  m.reg_write(UC_ARM_REG_R0,O);m.reg_write(UC_ARM_REG_R1,observer)
  m.reg_write(UC_ARM_REG_SP,0x180000);m.reg_write(UC_ARM_REG_LR,0x1f0001)
  m.emu_start(target|1,0x1f0000,count=30000)
  assert m.reg_read(UC_ARM_REG_PC)==0x1f0000 and not locked
  results.append((bytes(m.mem_read(O,0x150)),tuple(bytes(m.mem_read(a,HEAP_SIZE)) for a in (OLD,H1,H2)),tuple(trace)))
 return results
cases=0
for kind,offset in (('MapRender',0x24),('UiUpdate',0x48)):
 add='add'+kind+'Observer';remove='remove'+kind+'Observer'
 for flags in (0x11,0x10):
  for capacity,count in ((0,0),(4,4),(5,5),(499,499),(500,500),(600,400)):
   operations=[(add,0),(add,0xb0000),(add,0xb0000),(remove,0xc0000),(remove,0xa0000),(remove,0xb0000),(remove,0xb0000)]
   a=run(False,offset,flags,capacity,count,operations);b=run(True,offset,flags,capacity,count,operations)
   assert a==b,(kind,flags,capacity,count);cases+=1
  operations=[(add,0xa0000+i*4) for i in range(60)]+[(remove,0xa0000+i*4) for i in range(0,60,2)]+[(remove,0xa0000+i*4) for i in range(1,60,2)]
  assert run(False,offset,flags,0,0,operations)==run(True,offset,flags,0,0,operations);cases+=1
for name in ('setProjectionSizeListener','setUiStyleListener'):
 operations=[(name,0xa0000),(name,0xb0000),(name,0),(name,0)]
 assert run(False,0xb4,0,0,0,operations)==run(True,0xb4,0,0,0,operations);cases+=1
print(f'Original vs rebuilt UI observers: {cases} scenarios; locks, duplicate/null handling, growth, ordered removal and clear PASS')
