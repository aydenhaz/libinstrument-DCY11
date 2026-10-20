#!/usr/bin/env python3
"""Dump an explicit ELF32 vtable range, resolving imported relocation slots.

A zero raw word can be an imported function; never infer table length from it.
"""
import argparse, struct
from elftools.elf.elffile import ELFFile
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('elf');p.add_argument('start',type=lambda x:int(x,0));p.add_argument('words',type=lambda x:int(x,0))
a=p.parse_args()
with open(a.elf,'rb') as f:
 e=ELFFile(f)
 if e.elfclass!=32 or not e.little_endian: p.error('little-endian ELF32 required')
 rels={}
 for s in e.iter_sections():
  if s['sh_type'] not in ('SHT_REL','SHT_RELA'):continue
  syms=e.get_section(s['sh_link'])
  for r in s.iter_relocations():
   idx=r['r_info_sym'];sym=syms.get_symbol(idx) if idx else None
   rels[r['r_offset']]=(r['r_info_type'],sym.name if sym else '')
 def word(va):
  for s in e.iter_segments():
   if s['p_type']=='PT_LOAD' and s['p_vaddr']<=va and va+4<=s['p_vaddr']+s['p_filesz']:
    f.seek(s['p_offset']+va-s['p_vaddr']);return struct.unpack('<I',f.read(4))[0]
  raise ValueError(hex(va))
 for i in range(a.words):
  va=a.start+4*i;v=word(va);typ,name=rels.get(va,(None,''))
  print(f'{va:08x} {v:08x}'+(f' relocation={typ} {name}' if typ is not None else ''))
