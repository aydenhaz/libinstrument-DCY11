#!/usr/bin/env python3
"""Compile every translation unit and strictly link against the supplied ARM hosts."""
import argparse
from pathlib import Path
import subprocess
import sys

parser = argparse.ArgumentParser()
parser.add_argument('--ndk', type=Path, required=True)
parser.add_argument('--out', type=Path, default=Path('build/android-arm32'))
args = parser.parse_args()
root = Path(__file__).resolve().parent
out = args.out.resolve()
out.mkdir(parents=True, exist_ok=True)
cxx = args.ndk.resolve() / 'toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi21-clang++'
flags = ['-std=c++11', '-fPIC', '-mthumb']
for directory in ['include', 'stub_include', 'internal', 'internal/NS_MultiScreenBase', 'internal/NS_InstrumentPannelLarge']:
    flags.append('-I' + str(root / directory))
sources = sorted((root / 'src').glob('*.cpp')) + sorted((root / 'internal').rglob('*.cpp')) + sorted((root / 'expanded').glob('*.cpp'))
objects = []
failures = 0
with (out / 'compile.log').open('w') as log:
    for index, source in enumerate(sources):
        obj = out / f'{index}.o'
        result = subprocess.run([str(cxx), *flags, '-c', str(source), '-o', str(obj)], capture_output=True, text=True)
        log.write(f'{source.relative_to(root)}: {result.returncode}\n{result.stdout}{result.stderr}\n')
        if result.returncode:
            failures += 1
            print(result.stderr)
        else:
            objects.append(str(obj))
print(f'Compiled {len(objects)}/{len(sources)} source files')
if failures:
    sys.exit(1)
host_names = ['Common', 'GAdaptor', 'GAdaptorInterface', 'GFrame', 'GPlatformInterface', 'GSQuirrel', 'Gbl', 'base_utils', 'hsl']
hosts = [root / 'host_libs' / f'lib{name}.so' for name in host_names]
missing = [str(p) for p in hosts if not p.is_file()]
if missing:
    print('Missing host libraries:', ', '.join(missing))
    print('Run sh host_libs/restore_libGbl.sh if libGbl.so is compressed.')
    sys.exit(1)
output = out / 'libInstrumentPannelLarge.so'
output.unlink(missing_ok=True)
result = subprocess.run([str(cxx), '-shared', '-Wl,--no-undefined', '-Wl,--error-limit=0', '-Wl,-soname,libInstrumentPannelLarge.so', *objects, '-Wl,--no-as-needed', *map(str, hosts), '-o', str(output)], capture_output=True, text=True)
(out / 'link.log').write_text(result.stdout + result.stderr)
print('Strict link:', 'PASS' if result.returncode == 0 else 'FAIL')
if result.returncode:
    print(result.stderr)
    output.unlink(missing_ok=True)
sys.exit(result.returncode)
