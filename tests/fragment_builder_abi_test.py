"""Verify the V21 builder boundary with an Android NDK signature/link probe."""
import argparse
from pathlib import Path
import subprocess
import sys
import tempfile

root = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(root / 'tools'))
from inspect_packet_factory import ELF32
parser = argparse.ArgumentParser()
parser.add_argument('--ndk', type=Path, required=True)
args = parser.parse_args()
tools = args.ndk / 'toolchains/llvm/prebuilt/linux-x86_64/bin'
cxx = tools / 'armv7a-linux-androideabi21-clang++'
host = root / 'host_libs/libGFrame.so'
elf = ELF32(host)
# Host C2 clones both wrappers through slot 3, then retains method pairs.
assert elf.at(0x162bf2, 8).hex() == '3068c16830468847'
assert elf.at(0x162c38, 8).hex() == '2868c16828468847'
# Getters prove context and launch mode reside at +0x30 and +0x34.
assert elf.at(0x162d4c, 8).hex() == '006b7047406b7047'
# Builder D2 destroys the type Callable at +0x18 before the factory at +4.
assert elf.at(0x162c82, 4).hex() == '04f11800'
expected = {
    '_ZN9GNS_FRAME17CGFragmentBuilderC1EN3GPI7Closure8CallableIFPNS_10CGFragmentEvEEENS3_IFPvvEEEPFvS8_EiPNS_9CGContextE',
    '_ZN9GNS_FRAME17CGFragmentBuilderD1Ev',
    '_ZN9GNS_FRAME9CGPackage17_registerFragmentERK8CGStringPNS_17CGFragmentBuilderE',
}
with tempfile.TemporaryDirectory(prefix='builder-abi-') as directory:
    obj = Path(directory) / 'probe.o'
    subprocess.run([str(cxx), '-std=c++11', '-fPIC', '-mthumb',
                    '-I' + str(root / 'stub_include'), '-c',
                    str(root / 'tests/fragment_builder_signature_probe.cpp'),
                    '-o', str(obj)], check=True)
    undefined = subprocess.check_output([str(tools / 'llvm-nm'), '-u', str(obj)], text=True)
    assert expected <= {line.split()[-1] for line in undefined.splitlines()}
    assert expected <= set(elf.dynsym_names())
    subprocess.run([str(cxx), '-shared', '-static-libstdc++', '-Wl,--no-undefined',
                    str(obj), str(host), '-o', str(Path(directory) / 'probe.so')], check=True)
print('Builder host layout, ARM32 sizes, imported signatures and strict probe link: PASS')
