#!/usr/bin/env python3
"""Compile every translation unit and strictly link against the supplied ARM hosts."""
import argparse
from pathlib import Path
import subprocess
import sys

parser = argparse.ArgumentParser()
compiler = parser.add_mutually_exclusive_group(required=True)
compiler.add_argument('--ndk', type=Path)
compiler.add_argument('--cxx', type=Path, help='explicit compiler executable or target-configured launcher; not necessarily NDK')
parser.add_argument('--out', type=Path, default=Path('build/android-arm32'))
parser.add_argument('--compile-only', action='store_true', help='check all translation units without linking or publishing a shared library')
args = parser.parse_args()
root = Path(__file__).resolve().parent
out = args.out.resolve()
out.mkdir(parents=True, exist_ok=True)
# Clear stale successes before compilation or host validation can fail.
output = out / 'libInstrumentPannelLarge.so'
output.unlink(missing_ok=True)
candidate = out / 'libInstrumentPannelLarge.so.pending'
candidate.unlink(missing_ok=True)
cxx = args.cxx.resolve() if args.cxx else args.ndk.resolve() / 'toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi21-clang++'
(out / 'compiler.txt').write_text(str(cxx) + '\n')
# V21 production target: Android armeabi-v7a / ARMv7-A, Thumb, softfp calling
# convention. The NDK driver selects ARMv7/API 21 from its executable name;
# explicit arch/ABI flags keep this aligned with the supplied host DSOs.
flags = ['-std=c++11', '-fPIC', '-mthumb']
if args.ndk:
    flags += ['-march=armv7-a', '-mfloat-abi=softfp']
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
if args.compile_only:
    print('Compile-only: no shared library linked or published')
    sys.exit(0)
host_names = ['Common', 'GAdaptor', 'GAdaptorInterface', 'GFrame', 'GPlatformInterface', 'GSQuirrel', 'Gbl', 'base_utils', 'hsl']
hosts = [root / 'host_libs' / f'lib{name}.so' for name in host_names]
missing = [str(p) for p in hosts if not p.is_file()]
if missing:
    print('Missing host libraries:', ', '.join(missing))
    print('Run sh host_libs/restore_libGbl.sh if libGbl.so is compressed.')
    sys.exit(1)
result = subprocess.run([str(cxx), '-shared', '-Wl,--no-undefined', '-Wl,--error-limit=0', '-Wl,-soname,libInstrumentPannelLarge.so', *objects, '-Wl,--no-as-needed', *map(str, hosts), '-o', str(candidate)], capture_output=True, text=True)
(out / 'link.log').write_text(result.stdout + result.stderr)
print('Strict link:', 'PASS' if result.returncode == 0 else 'FAIL')
if result.returncode:
    print(result.stderr)
    candidate.unlink(missing_ok=True)
else:
    candidate.replace(output)
sys.exit(result.returncode)
