#!/usr/bin/env python3
"""Inventory object references against rebuilt definitions and supplied ELF hosts.

This is a symbol inventory, not a linker or an ABI/runtime compatibility check.
Requires pyelftools. Never use its result as permission to publish an SO.
"""
import argparse
import json
from collections import defaultdict
from pathlib import Path
from elftools.elf.elffile import ELFFile


def symbols(path, section):
    with path.open('rb') as stream:
        elf = ELFFile(stream)
        table = elf.get_section_by_name(section)
        if table is None:
            raise ValueError(f'{path}: missing {section}')
        identity = (elf.elfclass, elf.little_endian, elf['e_machine'])
        entries = []
        for sym in table.iter_symbols():
            if not sym.name or sym['st_info']['bind'] not in ('STB_GLOBAL', 'STB_WEAK', 'STB_GNU_UNIQUE'):
                continue
            entries.append((sym.name, sym['st_shndx'] != 'SHN_UNDEF',
                            sym['st_info']['bind'], sym['st_other']['visibility']))
        return identity, entries


def audit(objects, hosts):
    definitions, providers, references = defaultdict(list), defaultdict(list), defaultdict(list)
    identity = None
    for path in objects:
        current, entries = symbols(path, '.symtab')
        if identity is None:
            identity = current
        if current != identity:
            raise ValueError(f'{path}: incompatible ELF identity {current}, expected {identity}')
        for name, defined, binding, visibility in entries:
            if defined:
                definitions[name].append({'file': str(path), 'binding': binding})
            else:
                references[name].append({'file': str(path), 'binding': binding})
    for path in hosts:
        current, entries = symbols(path, '.dynsym')
        if current != identity:
            raise ValueError(f'{path}: incompatible ELF identity {current}, expected {identity}')
        for name, defined, binding, visibility in entries:
            if defined and visibility in ('STV_DEFAULT', 'STV_PROTECTED'):
                providers[name].append(str(path))
    result = []
    for name, uses in sorted(references.items()):
        if name in definitions:
            status = 'rebuilt'
        elif name in providers:
            status = 'host'
        elif all(use['binding'] == 'STB_WEAK' for use in uses):
            status = 'weak-unprovided'
        else:
            status = 'unprovided'
        result.append({'symbol': name, 'status': status, 'references': uses,
                       'providers': providers.get(name, [])})
    counts = {key: sum(row['status'] == key for row in result)
              for key in ('rebuilt', 'host', 'weak-unprovided', 'unprovided')}
    return {'scope': 'Symbol inventory only; not an Android link or ABI validation',
            'elf_identity': identity, 'objects': [str(p) for p in objects],
            'hosts': [str(p) for p in hosts], 'counts': counts, 'symbols': result}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--objects', type=Path, required=True, help='directory of current build objects')
    parser.add_argument('--hosts', type=Path, required=True, help='directory of host shared libraries')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    objects, hosts = sorted(args.objects.glob('*.o')), sorted(args.hosts.glob('*.so'))
    if not objects or not hosts:
        parser.error('object and host sets must both be nonempty')
    report = audit(objects, hosts)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report['counts']))
    for row in report['symbols']:
        if row['status'] == 'unprovided':
            print(row['symbol'])


if __name__ == '__main__':
    main()
