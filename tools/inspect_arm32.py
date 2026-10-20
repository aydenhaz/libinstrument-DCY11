#!/usr/bin/env python3
"""Print Thumb disassembly with ELF32 PLT names and literal addresses (Capstone)."""
import argparse, struct, subprocess
from pathlib import Path
import capstone

p=argparse.ArgumentParser();p.add_argument('elf',type=Path);p.add_argument('start',type=lambda s:int(s,0));p.add_argument('size',type=lambda s:int(s,0));a=p.parse_args()
b=a.elf.read_bytes()
if b[:5] != b'\x7fELF\x01':raise SystemExit('ELF32 required')
phoff,shoff=struct.unpack_from('<II',b,28);phents,phnum,shents,shnum,shstr=struct.unpack_from('<5H',b,42)
segments=[struct.unpack_from('<8I',b,phoff+i*phents) for i in range(phnum)]
def read(va,n):
 for typ,off,base,_,size,*_ in segments:
  if typ==1 and base<=va and va+n<=base+size:return b[off+va-base:off+va-base+n]
 raise ValueError(hex(va))
sections=[struct.unpack_from('<10I',b,shoff+i*shents) for i in range(shnum)]
strsec=sections[shstr];names=b[strsec[4]:strsec[4]+strsec[5]]
def name(s):return names[s[0]:names.find(b'\0',s[0])].decode()
ss={name(s):s for s in sections};sym=ss['.dynsym'];ds=ss['.dynstr'];strings=b[ds[4]:ds[4]+ds[5]]
syms=[]
for off in range(sym[4],sym[4]+sym[5],16):
 no=struct.unpack_from('<I',b,off)[0];syms.append(strings[no:strings.find(b'\0',no)].decode())
r=ss['.rel.plt'];plt=ss['.plt'];calls={}
for i,off in enumerate(range(r[4],r[4]+r[5],8)):
 _,info=struct.unpack_from('<II',b,off);calls[plt[3]+20+12*i]=syms[info>>8]
md=capstone.Cs(capstone.CS_ARCH_ARM,capstone.CS_MODE_THUMB|capstone.CS_MODE_LITTLE_ENDIAN)
for ins in md.disasm(read(a.start,a.size),a.start):
 extra=''
 if ins.mnemonic in ('bl','blx','b.w') and ins.op_str.startswith('#'):
  target=int(ins.op_str[1:],0);extra=calls.get(target,'')
 if ins.mnemonic.startswith('ldr') and '[pc,' in ins.op_str:
  import re
  m=re.search(r'\[pc, #([^\]]+)\]',ins.op_str)
  if m:
   va=((ins.address+4)&~3)+int(m[1],0)
   try:extra='literal '+hex(struct.unpack('<I',read(va,4))[0])
   except ValueError:pass
 print(f'{ins.address:08x} {ins.mnemonic:10s} {ins.op_str:40s} {extra}')
