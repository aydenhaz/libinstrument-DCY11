"""Reject fake location storage while preserving typed host declarations."""
import argparse
from pathlib import Path
import subprocess
import tempfile
import sys
root = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(root / 'tools'))
from inspect_packet_factory import ELF32, plt_symbol
parser = argparse.ArgumentParser()
parser.add_argument('--cxx', default='g++')
args = parser.parse_args()
elf = ELF32(root / 'host_libs/libhsl.so')
# CarLoc host code reads a field immediately beyond the removed 16-byte type.
assert elf.at(0x40f418, 2).hex() == '0869'  # ldr r0,[r1,#0x10]
# LocInfo copy prefix is 0x128 bytes; suffix is +0x12c, length 0x560.
assert elf.at(0x38f51c, 4).hex() == '4ff49472'
assert elf.at(0x38f534, 12).hex() == '05f5967004f596714ff4ac62'
assert 'memcpy' in plt_symbol(elf, 0x12701c)[1]
assert 'memcpy' in plt_symbol(elf, 0x1267e8)[1]
with tempfile.TemporaryDirectory() as directory:
    src = Path(directory) / 'probe.cpp'
    command = [args.cxx, '-std=c++11', '-fsyntax-only', '-I' + str(root / 'stub_include'), str(src)]
    src.write_text('#include <hsl/HSL.h>\n#include <hsl/Pos.h>\n'
                   'void probe(bl::CarLoc& car, bl::LocInfo& loc, hsl::Pos* pos) {\n'
                   'hsl::HSL::GetCarPosition(car, static_cast<bl::MapEngineID>(1));\n'
                   'pos->GetLocInfo(loc); }\n')
    subprocess.run(command, check=True)
    for name in ('CarLoc', 'LocInfo'):
        src.write_text('#include <hsl/HSL.h>\nvoid bad() { bl::' + name + ' object; }\n')
        result = subprocess.run(command, capture_output=True, text=True)
        assert result.returncode != 0 and 'incomplete' in result.stderr, result.stderr
print('Host layout bounds and forbidden fake location construction: PASS')
