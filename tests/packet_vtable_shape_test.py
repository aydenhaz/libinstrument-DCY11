#!/usr/bin/env python3
"""Compare a freestanding 32-bit C++ hierarchy's vtable shape with V21."""
import argparse
import os
from pathlib import Path
import re
import struct
import subprocess
import tempfile

source = Path(__file__).with_name("packet_vtable_shape_probe.cpp")
relocation_section = ".rel.data.rel.ro._ZTV6Packet"
parser = argparse.ArgumentParser(description=__doc__)
target = parser.add_mutually_exclusive_group()
target.add_argument("--arm", action="store_true", help="compile ARMv7 Thumb softfp GNU object")
target.add_argument("--ndk", type=Path, help="compile Android ARMv7 Thumb softfp object")
args = parser.parse_args()
if args.ndk:
    toolchain = args.ndk.resolve() / "toolchains/llvm/prebuilt/linux-x86_64/bin"
    compiler = str(toolchain / "armv7a-linux-androideabi21-clang++")
    objcopy = str(toolchain / "llvm-objcopy")
elif args.arm:
    compiler = os.environ.get("ARM_CXX", "arm-linux-gnueabihf-g++")
    objcopy = os.environ.get("ARM_OBJCOPY", "arm-linux-gnueabihf-objcopy")
else:
    compiler = os.environ.get("CXX", "g++")
    objcopy = os.environ.get("OBJCOPY", "objcopy")
relocation = "R_ARM_ABS32" if args.arm or args.ndk else "R_386_32"
expected = {
    0x04: "_ZTI6Packet", 0x08: "_ZN6PacketD1Ev", 0x0c: "_ZN6PacketD0Ev",
    0x10: "_ZN5Async8executorEv", 0x14: "_ZN5Async8uiThreadEv",
    0x18: "_ZN3Pkg8supportsEv", 0x1c: "_ZN3Pkg6activeEv",
    0x20: "_ZN6Packet7initEndEv", 0x24: "_ZN3Pkg5slot7Ev",
    0x28: "_ZN3Pkg5slot8Ev", 0x2c: "_ZN6Packet7destroyEv",
    0x30: "_ZN6Packet7preloadEv", 0x34: "_ZN3Pkg6slot11Ev",
    0x38: "_ZN3Pkg6slot12Ev", 0x3c: "_ZN3Pkg6slot13Ev",
    0x40: "_ZN6Packet8postLoadEv", 0x44: "_ZN6Packet11postAlreadyEv",
    0x4c: "_ZTI6Packet", 0x50: "_ZThn64_N6PacketD1Ev",
    0x54: "_ZThn64_N6PacketD0Ev", 0x5c: "_ZTI6Packet",
    0x60: "_ZN6MapObs9mapStatusEv", 0x64: "_ZThn192_N6PacketD1Ev",
    0x68: "_ZThn192_N6PacketD0Ev", 0x6c: "_ZN5Cover10onBusinessEv",
    0x74: "_ZTI6Packet", 0x78: "_ZThn196_N6PacketD1Ev",
    0x7c: "_ZThn196_N6PacketD0Ev",
    0x80: "_ZThn4_N5Cover10onBusinessEv",
}


def section_bytes(obj, section, destination):
    subprocess.run([objcopy,
                    "--dump-section=" + section + "=" + str(destination), str(obj)],
                   check=True, capture_output=True)
    return destination.read_bytes()


with tempfile.TemporaryDirectory(prefix="ipl-vtable-") as directory:
    tmp = Path(directory)
    obj = tmp / "shape.o"
    flags = (["-mthumb", "-march=armv7-a", "-mfloat-abi=softfp"]
             if args.arm or args.ndk else ["-m32"])
    subprocess.run([compiler, "-std=c++11", "-fPIC", *flags,
                    "-c", str(source), "-o", str(obj)], check=True)
    output = subprocess.check_output([os.environ.get("READELF", "readelf"),
                                      "-rW", str(obj)], text=True)
    heading = "Relocation section '" + relocation_section + "'"
    lines = output.split(heading, 1)[1].split("Relocation section '", 1)[0]
    entries = {}
    for line in lines.splitlines():
        match = re.match(r"^([0-9a-f]{8})\s+\S+\s+" + relocation +
                         r"\s+\S+\s+(\S+)", line)
        if match:
            entries[int(match.group(1), 16)] = match.group(2)
    # Clang may select the complete destructor's D2 alias for slot zero.
    # Both symbols designate the same non-deleting destructor entry point.
    assert entries.get(0x08) in ("_ZN6PacketD1Ev", "_ZN6PacketD2Ev"), entries
    expected[0x08] = entries[0x08]
    assert entries == expected, (entries, expected)

    data = section_bytes(obj, ".data.rel.ro._ZTV6Packet", tmp / "vtable.bin")
    assert len(data) == 0x84, len(data)
    assert [struct.unpack_from("<i", data, offset)[0]
            for offset in (0, 0x48, 0x58, 0x70)] == [0, -0x40, -0xc0, -0xc4]
    assert [offset + 8 for offset in (0, 0x48, 0x58, 0x70)] == [
        0x08, 0x50, 0x60, 0x78]
    rtti = section_bytes(obj, ".data.rel.ro._ZTI6Packet", tmp / "rtti.bin")
    cover_rtti = section_bytes(obj, ".data.rel.ro._ZTI5Cover", tmp / "cover-rtti.bin")
    assert (struct.unpack_from("<I", rtti, 0x14)[0],
            struct.unpack_from("<I", rtti, 0x1c)[0]) == (2, 0xc002)
    assert (struct.unpack_from("<I", cover_rtti, 0x14)[0],
            struct.unpack_from("<I", cover_rtti, 0x1c)[0]) == (2, 0x402)

print(("Android ARMv7 Thumb softfp" if args.ndk else
       "GNU ARMv7 Thumb softfp" if args.arm else "x86 32-bit") +
      " Packet vtable/RTTI shape: PASS")
