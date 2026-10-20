#!/usr/bin/env python3
"""Read ARM32 Itanium RTTI, including imported base-type relocations."""
import argparse,json,struct
from elftools.elf.elffile import ELFFile
p=argparse.ArgumentParser(description=__doc__);p.add_argument('elf');p.add_argument('addresses',nargs='+',type=lambda s:int(s,0));a=p.parse_args()
with open(a.elf,'rb') as f:
 e=ELFFile(f)
 if e.elfclass!=32 or not e.little_endian:p.error('little-endian ELF32 required')
 rels={}
 for s in e.iter_sections():
  if s['sh_type'] not in ('SHT_REL','SHT_RELA'):continue
  symbols=e.get_section(s['sh_link'])
  for r in s.iter_relocations():
   if r['r_info_sym']:
    sym=symbols.get_symbol(r['r_info_sym'])
    rels[r['r_offset']]=(r['r_info_type'],sym.name,sym['st_value'],sym['st_shndx']!='SHN_UNDEF')
 def read(addr,n):
  for seg in e.iter_segments():
   if seg['p_type']=='PT_LOAD' and seg['p_vaddr']<=addr and addr+n<=seg['p_vaddr']+seg['p_filesz']:
    f.seek(seg['p_offset']+addr-seg['p_vaddr']);return f.read(n)
  raise ValueError(hex(addr))
 def word(addr):return struct.unpack('<I',read(addr,4))[0]
 def name(addr):
  value=bytearray()
  for i in range(2048):
   ch=read(addr+i,1)
   if ch==b'\0':return value.decode('utf-8')
   value.extend(ch)
  raise ValueError('unterminated RTTI name')
 def pointer(addr):
  raw=word(addr)
  if addr not in rels:return raw,None
  typ,symbol,value,defined=rels[addr]
  if not defined:return None,symbol
  if typ==2:return (value+raw)&0xffffffff,None # R_ARM_ABS32: S+A
  if typ==21:return value,None # R_ARM_GLOB_DAT: S
  raise ValueError('unsupported symbolic pointer relocation '+str(typ))
 def type_name(addr):
  at,external=pointer(addr+4)
  if external:raise ValueError('external RTTI name '+external)
  if not at:raise ValueError('null RTTI name at '+hex(addr))
  return name(at)
 def ref(addr):
  raw,external=pointer(addr)
  if external:return {'imported_type':external}
  if not raw:raise ValueError('null base RTTI at '+hex(addr))
  return {'address':hex(raw),'name':type_name(raw)}
 def inspect(addr):
  kind=rels[addr][1] if addr in rels else '';result={'address':hex(addr),'name':type_name(addr),'typeinfo_class':kind,'bases':[]}
  if '__vmi_class_type_info' in kind:
   count=word(addr+12);result['flags']=word(addr+8)
   if count>128:raise ValueError('implausible base count')
   for i in range(count):
    at=addr+16+8*i;bits=word(at+4);signed=bits if bits<0x80000000 else bits-0x100000000
    base=ref(at);base.update(offset_or_vtable_offset=signed>>8,public=bool(bits&2),virtual=bool(bits&1));result['bases'].append(base)
  elif '__si_class_type_info' in kind:result['bases'].append(ref(addr+8))
  elif '__class_type_info' not in kind:raise ValueError('unrecognized RTTI vptr relocation: '+kind)
  return result
 print(json.dumps([inspect(addr) for addr in a.addresses],indent=2))
