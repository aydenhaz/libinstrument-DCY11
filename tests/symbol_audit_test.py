#!/usr/bin/env python3
"""Check inventory classification using real ELF objects and shared libraries."""
import importlib.util
from pathlib import Path
import subprocess
import tempfile
import unittest

spec = importlib.util.spec_from_file_location('audit', Path(__file__).resolve().parents[1] / 'tools/audit_elf_symbols.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class AuditTest(unittest.TestCase):
    def test_definitions_visibility_and_weak_references(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / 'caller.c').write_text('''
extern void exported(void), hidden(void), missing(void), local(void);
extern void optional(void) __attribute__((weak));
void caller(void) { exported(); hidden(); missing(); local(); if(optional) optional(); }
''')
            (root / 'local.c').write_text('void local(void) {}\n')
            (root / 'host.c').write_text('void exported(void) {}\n__attribute__((visibility("hidden"))) void hidden(void) {}\n')
            for name in ('caller', 'local'):
                subprocess.run(['cc', '-fPIC', '-c', str(root / (name + '.c')), '-o', str(root / (name + '.o'))], check=True)
            subprocess.run(['cc', '-shared', '-fPIC', '-nostdlib', str(root / 'host.c'), '-o', str(root / 'host.so')], check=True)
            report = audit.audit([root / 'caller.o', root / 'local.o'], [root / 'host.so'])
            rows = {row['symbol']: row for row in report['symbols']}
            self.assertEqual(rows['exported']['status'], 'host')
            self.assertEqual(rows['local']['status'], 'rebuilt')
            self.assertEqual(rows['missing']['status'], 'unprovided')
            self.assertEqual(rows['hidden']['status'], 'unprovided')
            self.assertEqual(rows['optional']['status'], 'weak-unprovided')
            self.assertIn(str(root / 'host.so'), rows['exported']['providers'])


if __name__ == '__main__':
    unittest.main()
