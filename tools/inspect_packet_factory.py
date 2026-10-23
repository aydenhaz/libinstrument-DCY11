#!/usr/bin/env python3
"""Inspect the original ARM32 Packet factory contract without executing the SO.

Addresses below are image virtual addresses from the 7.5 V21 ELF. This
checks static construction evidence; it does not certify rebuilt code.
"""
import argparse
import json
from pathlib import Path
import struct


class ELF32:
    def __init__(self, path):
        self.data = path.read_bytes()
        if self.data[:6] != b"\x7fELF\x01\x01":
            raise ValueError("expected little-endian ELF32")
        phoff, shoff = struct.unpack_from("<II", self.data, 28)
        phentsize, phnum, shentsize, shnum, shstrndx = struct.unpack_from(
            "<5H", self.data, 42
        )
        self.segments = [
            struct.unpack_from("<8I", self.data, phoff + i * phentsize)
            for i in range(phnum)
        ]
        sections = [
            struct.unpack_from("<10I", self.data, shoff + i * shentsize)
            for i in range(shnum)
        ]
        names = sections[shstrndx]
        name_data = self.data[names[4] : names[4] + names[5]]
        self.sections = {}
        for entry in sections:
            index = entry[0]
            name = name_data[index : name_data.index(b"\0", index)].decode()
            self.sections[name] = entry

    def at(self, address, size):
        for kind, offset, start, _, file_size, *_ in self.segments:
            if kind == 1 and start <= address and address + size <= start + file_size:
                position = offset + address - start
                return self.data[position : position + size]
        raise ValueError(f"address outside file-backed PT_LOAD: 0x{address:x}")

    def u32(self, address):
        return struct.unpack("<I", self.at(address, 4))[0]

    def rel_dyn(self):
        section = self.sections[".rel.dyn"]
        return {
            address: info & 0xff
            for address, info in struct.iter_unpack(
                "<II", self.data[section[4] : section[4] + section[5]]
            )
        }


def inspect(path):
    elf = ELF32(path)
    init = elf.sections[".init_array"]
    init_array = [
        elf.u32(init[3] + i) for i in range(0, init[5], 4)
    ]
    # Four subobjects: CGFragmentPackage at 0, its CGObjMem at +0x40,
    # CoverUtil at +0xc0 and its second observer at +0xc4.
    groups = [
        ("package", 0x6B7F8, 0x6B800, 0),
        ("package_objmem", 0x6B840, 0x6B848, -0x40),
        ("cover_map_observer", 0x6B850, 0x6B858, -0xC0),
        ("cover_business_observer", 0x6B868, 0x6B870, -0xC4),
    ]
    ends = [0x6B840, 0x6B850, 0x6B868, 0x6B87C]
    relocs = elf.rel_dyn()
    tables = []
    for (name, head, point, top), end in zip(groups, ends):
        words = [elf.u32(a) for a in range(point, end, 4)]
        tables.append({
            "subobject": name,
            "offset_to_top": struct.unpack("<i", elf.at(head, 4))[0],
            "typeinfo": hex(elf.u32(head + 4)),
            "address_point": hex(point),
            "slots": [hex(w) for w in words],
            "slot_relocations": [relocs.get(a) for a in range(point, end, 4)],
        })
    return {
        "init_array": [hex(a) for a in init_array],
        "factory_thumb": "0x2eb09",
        "factory_alloc_immediate": elf.at(0x2EB0E, 4).hex(),
        "factory_proxy_clear": elf.at(0x2EB26, 4).hex(),
        # Thumb ADD r0,pc at 0x2eb46 observes PC=0x2eb4a.
        "factory_singleton_slot": hex(0x2EB4A + elf.u32(0x2EB54)),
        "factory_singleton_store": elf.at(0x2EB48, 2).hex(),
        "short_virtual_bodies": {
            hex(address): elf.at(address, size).hex()
            for address, size in (
                (0x2EB7E, 4), (0x2EB82, 4),
                (0x2F2EC, 2), (0x2F2EE, 2),
                (0x2F5F4, 2), (0x2F5F6, 2), (0x2F5F8, 2),
            )
        },
        "tables": tables,
    }


def check(report):
    expected_init = [0x14CF9, 0x14D31, 0x14D69, 0x14D9D, 0x14E41]
    if report["init_array"] != [hex(a) for a in expected_init]:
        raise ValueError("V21 initializer list differs")
    if report["factory_alloc_immediate"] != "4ff4b670":
        raise ValueError("factory allocation instruction differs from 0x16c")
    if report["factory_proxy_clear"] != "c5f86801":
        raise ValueError("factory proxy clear instruction differs")
    if report["factory_singleton_slot"] != "0x6fd48" or report["factory_singleton_store"] != "0560":
        raise ValueError("factory singleton publication differs")
    tops = [0, -0x40, -0xC0, -0xC4]
    slots = [
        [0x2EB59, 0x2EB6F],
        [0x2FCA1, 0x2FCA7],
        [0x17D15, 0x2FCAD, 0x2FCB3, 0x17E9D],
        [0x2FCB9, 0x2FCBF, 0x18069],
    ]
    for table, top, prefix in zip(report["tables"], tops, slots):
        if table["offset_to_top"] != top or table["typeinfo"] != "0x6b87c":
            raise ValueError(f"vtable header differs: {table['subobject']}")
        if table["slots"][:len(prefix)] != [hex(a) for a in prefix]:
            raise ValueError(f"vtable slots differ: {table['subobject']}")
        for i, relocation in enumerate(table["slot_relocations"]):
            # Two imported CGAsyncObject methods in the primary group use ABS32.
            expected = 2 if table["subobject"] == "package" and i in (2, 3) else 23
            if relocation != expected:
                raise ValueError(f"unexpected relocation in {table['subobject']} slot {i}")
    if report["tables"][0]["slots"][4:6] != ["0x2eb7f", "0x2eb83"]:
        raise ValueError("package vtable prefix differs")
    for i, target in {
        6: 0x2EB89, 9: 0x2F2F1, 10: 0x2F465,
        14: 0x2F5FD, 15: 0x2FC31,
    }.items():
        if report["tables"][0]["slots"][i] != hex(target):
            raise ValueError(f"package lifecycle slot {i} differs")
    expected_short = {
        "0x2eb7e": "00207047", "0x2eb82": "01207047",
        "0x2f2ec": "7047", "0x2f2ee": "7047",
        "0x2f5f4": "7047", "0x2f5f6": "7047", "0x2f5f8": "7047",
    }
    if report["short_virtual_bodies"] != expected_short:
        raise ValueError("short Packet virtual bodies differ")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "elf", nargs="?", type=Path,
        default=Path(__file__).resolve().parents[1] / "reference/v21/libInstrumentPannelLarge.so",
    )
    parser.add_argument("--check-v21", action="store_true")
    args = parser.parse_args()
    report = inspect(args.elf)
    if args.check_v21:
        check(report)
        report["v21_contract"] = "PASS"
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
