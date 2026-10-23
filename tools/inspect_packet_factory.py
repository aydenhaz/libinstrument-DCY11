#!/usr/bin/env python3
"""Inspect the original ARM32 Packet factory contract without executing the SO.

Addresses below are image virtual addresses from the 7.5 V21 ELF. This
checks static construction evidence; it does not certify rebuilt code.
"""
import argparse
import hashlib
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

    def dynsym_names(self):
        sym = self.sections[".dynsym"]
        strings = self.sections[".dynstr"]
        names = self.data[strings[4] : strings[4] + strings[5]]
        result = []
        for offset in range(sym[4], sym[4] + sym[5], 16):
            index = struct.unpack_from("<I", self.data, offset)[0]
            result.append(names[index : names.index(b"\0", index)].decode())
        return result

    def relocation_symbols(self, name):
        section = self.sections[name]
        symbols = self.dynsym_names()
        return {
            address: (info & 0xff, symbols[info >> 8])
            for address, info in struct.iter_unpack(
                "<II", self.data[section[4] : section[4] + section[5]]
            )
        }


def thumb_bl_target(elf, address):
    first, second = struct.unpack("<HH", elf.at(address, 4))
    if first >> 11 != 0x1e or second >> 14 != 0x3:
        raise ValueError("expected Thumb long branch")
    s, j1, j2 = (first >> 10) & 1, (second >> 13) & 1, (second >> 11) & 1
    i1, i2 = 1 ^ j1 ^ s, 1 ^ j2 ^ s
    offset = ((s << 24) | (i1 << 23) | (i2 << 22)
              | ((first & 0x3ff) << 12) | ((second & 0x7ff) << 1))
    if s:
        offset -= 1 << 25
    return address + 4 + offset


def plt_symbol(elf, target):
    plt = elf.sections[".plt"][3]
    position, remainder = divmod(target - (plt + 20), 12)
    if remainder or position < 0:
        raise ValueError("target is not an ARM PLT entry")
    rel_plt = elf.sections[".rel.plt"]
    address, info = struct.unpack_from(
        "<II", elf.data, rel_plt[4] + 8 * position
    )
    return address, elf.dynsym_names()[info >> 8]


def inspect_host(path):
    elf = ELF32(path)
    # BL from CGFragmentPackage(CGContext*) at 0x1b7614 to CGPackage C2 PLT.
    target = thumb_bl_target(elf, 0x1B7614)
    address, base_constructor = plt_symbol(elf, target)
    got = 0x1B761E + elf.u32(0x1B762C)
    vtable_relocation = elf.relocation_symbols(".rel.dyn").get(got)
    relocations = elf.relocation_symbols(".rel.dyn")
    primary_point = 0x3D8AB0
    secondary_point = 0x3D8AF8
    async_ctor = plt_symbol(elf, thumb_bl_target(elf, 0x1B4442) & ~3)[1]
    package_string_ctor = plt_symbol(elf, thumb_bl_target(elf, 0x1B448A) & ~3)[1]
    package_intent_ctor = plt_symbol(elf, thumb_bl_target(elf, 0x1B4496) & ~3)[1]
    package_string_dtor = plt_symbol(elf, thumb_bl_target(elf, 0x1B433C) & ~3)[1]
    package_intent_dtor = plt_symbol(elf, thumb_bl_target(elf, 0x1B4334) & ~3)[1]
    return {
        "fragment_package_constructor": "0x1b7610",
        "base_constructor_plt": hex(target),
        "base_constructor_symbol": base_constructor,
        "base_constructor_got": hex(address),
        "fragment_vtable_got": hex(got),
        "fragment_vtable_relocation": vtable_relocation,
        "fragment_vptr_stores": elf.at(0x1B761E, 10).hex(),
        "package_prefix": {
            "async_ctor_symbol": async_ctor,
            "string_ctor_symbol": package_string_ctor,
            "intent_ctor_symbol": package_intent_ctor,
            "intent_dtor_symbol": package_intent_dtor,
            "string_dtor_symbol": package_string_dtor,
            "async_ctor_sha256": hashlib.sha256(elf.at(0x1FD92C, 68)).hexdigest(),
            "package_ctor_sha256": hashlib.sha256(elf.at(0x1B4434, 300)).hexdigest(),
            "package_dtor_sha256": hashlib.sha256(elf.at(0x1B41F4, 420)).hexdigest(),
        },
        "fragment_vtable": {
            "primary_point": hex(primary_point),
            "primary_imports": [relocations.get(primary_point + 4 * i)
                                for i in range(4)],
            "primary_local_slots": [hex(elf.u32(primary_point + 4 * i))
                                    for i in range(4, 16)],
            "primary_local_bodies": elf.at(0x1B43A8, 0x1C).hex(),
            "secondary_point": hex(secondary_point),
            "secondary_offset_to_top": struct.unpack(
                "<i", elf.at(secondary_point - 8, 4))[0],
            "secondary_imports": [relocations.get(secondary_point + 4 * i)
                                  for i in range(2)],
        },
    }


def inspect_cover_destructor(elf):
    # Thumb BLX uses an aligned PC; the common BL decoder's raw result is
    # two bytes ahead for this halfword-aligned call at 0x17e62.
    string_dtor_plt = thumb_bl_target(elf, 0x17E62) & ~3
    _, string_dtor_symbol = plt_symbol(elf, string_dtor_plt)
    body = elf.at(0x17DFC, 0x7C)
    return {
        "body_sha256": hashlib.sha256(body).hexdigest(),
        "member_teardown": elf.at(0x17E5E, 0x10).hex(),
        "surface_string_dtor_plt": hex(string_dtor_plt),
        "surface_string_dtor_symbol": string_dtor_symbol,
        "text_dtor_local": hex(thumb_bl_target(elf, 0x17E6A)),
        "packet_cover_dtor_call": hex(thumb_bl_target(elf, 0x2EB60)),
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
        # The Packet teardown at 0x2f41a..0x2f424 zeros only the factory's
        # singleton. Its PC-relative literal is at 0x2f460; the module
        # singleton allocated by onInitEnd lives at a different BSS address.
        "module_lifetime": {
            "module_singleton_slot": hex(0x2EBE8 + elf.u32(0x2EEBC)),
            "on_destroy_clear_slot": hex(0x2F426 + elf.u32(0x2F460)),
            "on_destroy_tail": elf.at(0x2F41A, 12).hex(),
        },
        "module_log_tags": {
            "on_init_end": elf.at(0x2EBBA, 4).hex(),
            "module_reg": elf.at(0x2EC2A, 4).hex(),
            "module_unreg": elf.at(0x2DFD6, 4).hex(),
            "query": elf.at(0x2E062, 4).hex(),
            "query_result": elf.at(0x2E0AA, 2).hex(),
            "query_null_branch_and_publish": elf.at(0x2E08A, 0x12).hex(),
        },
        "opening_fragment": {
            "allocation_immediate": elf.at(0x2ECFC, 4).hex(),
            "constructor_target": hex(thumb_bl_target(elf, 0x2ED0A)),
            "constructor_tag": elf.at(0x2ED0C + elf.u32(0x2EED4), 12)
                                  .rstrip(b"\0").decode("ascii"),
            "host_fragment_base": plt_symbol(
                elf, thumb_bl_target(elf, 0x41968) & ~3)[1],
            "runtime_info_receiver_and_arg": elf.at(0x2ED42, 8).hex(),
            "embedded_runtime_info": plt_symbol(
                elf, thumb_bl_target(elf, 0x2ED5C) & ~3)[1],
            "set_context": plt_symbol(
                elf, thumb_bl_target(elf, 0x2F182) & ~3)[1],
            "set_type": plt_symbol(
                elf, thumb_bl_target(elf, 0x2F18C) & ~3)[1],
            "activity_set_fragment": plt_symbol(
                elf, thumb_bl_target(elf, 0x2F1A0) & ~3)[1],
            "cover_binding_tail": elf.at(0x2F1BA, 16).hex(),
        },
        "short_virtual_bodies": {
            hex(address): elf.at(address, size).hex()
            for address, size in (
                (0x2EB7E, 4), (0x2EB82, 4),
                (0x2F2EC, 2), (0x2F2EE, 2),
                (0x2F5F4, 2), (0x2F5F6, 2), (0x2F5F8, 2),
            )
        },
        "cover_destructor": inspect_cover_destructor(elf),
        "packet_adjustor_thunks": elf.at(0x2FCA0, 0x24).hex(),
        "cover_business_adjustor": elf.at(0x18068, 0x0E).hex(),
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
    if report["module_lifetime"] != {
        "module_singleton_slot": "0x6fd4c",
        "on_destroy_clear_slot": "0x6fd48",
        "on_destroy_tail": "0020cbf868010f4979440860",
    }:
        raise ValueError("Packet teardown/module singleton boundary differs from V21")
    if report["module_log_tags"] != {
        "on_init_end": "4ff4977c",  # 0x12e
        "module_reg": "4ff0220c",  # 0x22
        "module_unreg": "4ff04b0c",  # 0x4b
        "query": "4ff09f0c",  # 0x9f
        "query_result": "a622",  # 0xa6 (166 decimal)
        "query_null_branch_and_publish": "baf1000f21d009f10400ab46b046caf80000",
    }:
        raise ValueError("Packet/module ALC tags or QueryInterface branch differ from V21")
    if report["opening_fragment"] != {
        "allocation_immediate": "4ff48860",
        "constructor_target": "0x41954",
        "constructor_tag": "HMI_OPENING",
        "host_fragment_base": "_ZN9GNS_FRAME10CGFragmentC2ENS0_11ELaunchModeE",
        "runtime_info_receiver_and_arg": "06f5617006f56071",
        "embedded_runtime_info": "_ZN2bl17ObjectRuntimeInfoC1EPNS_8BLObjectE",
        "set_context": "_ZN9GNS_FRAME10CGFragment10setContextEPNS_9CGContextE",
        "set_type": "_ZN9GNS_FRAME10CGFragment7setTypeEPv",
        "activity_set_fragment": "_ZN9GNS_FRAME10CGActivity11setFragmentEiPNS_10CGFragmentERKNS_8CGIntentE",
        "cover_binding_tail": "00f1c00130461bb0bde8000fbde8f040",
    }:
        raise ValueError("Opening Fragment allocation/host wiring differs from V21")
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
    expected_cover_dtor = {
        "body_sha256": "3396d429a464264b1dad5c2db36289f7c0fb04148b7b0722425d8f5aee69c103",
        "member_teardown": "04f14400fbf77aef04f11800fef716fa",
        "surface_string_dtor_plt": "0x13d58",
        "surface_string_dtor_symbol": "_ZN8CGStringD1Ev",
        "text_dtor_local": "0x1629a",
        "packet_cover_dtor_call": "0x17dfc",
    }
    if report["cover_destructor"] != expected_cover_dtor:
        raise ValueError("CoverUtil destructor boundary differs from V21")
    if report["packet_adjustor_thunks"] != (
        "4038fef759bf4038fef761bf"
        "c038fef753bfc038fef75bbf"
        "c438fef74dbfc438fef755bf"
    ) or report["cover_business_adjustor"] != "80b56f460438fff715ff012080bd":
        raise ValueError("Packet/Cover virtual this-adjustment differs from V21")


def check_host(report):
    if report["base_constructor_symbol"] != "_ZN9GNS_FRAME9CGPackageC2EPNS_9CGContextE":
        raise ValueError("unexpected CGFragmentPackage base constructor")
    if report["fragment_vtable_got"] != "0x3e9c04" or report["fragment_vtable_relocation"] != (
        21, "_ZTVN9GNS_FRAME17CGFragmentPackageE"
    ):
        raise ValueError("unexpected CGFragmentPackage vtable GOT")
    if report["fragment_vptr_stores"] != "01f15002026408310160":
        raise ValueError("host +0x40/+0 virtual pointer setup differs")
    if report["package_prefix"] != {
        "async_ctor_symbol": "_ZN9GNS_FRAME13CGAsyncObjectC2Ev",
        "string_ctor_symbol": "_ZN8CGStringC1Ev",
        "intent_ctor_symbol": "_ZN9GNS_FRAME8CGIntentC2Ev",
        "intent_dtor_symbol": "_ZN9GNS_FRAME8CGIntentD2Ev",
        "string_dtor_symbol": "_ZN8CGStringD1Ev",
        "async_ctor_sha256": "530eca109186c252ee471c2a2a238a1224bd3f90f0c3ff184d16670f57ce658b",
        "package_ctor_sha256": "7b2e2c3793933fa5a202a6128ecbd342ee158b2bede114c9f2c3f312dc562862",
        "package_dtor_sha256": "234170c2c6ca1c5bc41569cd79455acf2f7cc95dfc79c292be81eb5fb5f25f8f",
    }:
        raise ValueError("host CGPackage owned prefix differs from V21")
    table = report["fragment_vtable"]
    if table != {
        "primary_point": "0x3d8ab0",
        "primary_imports": [
            (2, "_ZN9GNS_FRAME9CGPackageD2Ev"),
            (2, "_ZN9GNS_FRAME17CGFragmentPackageD0Ev"),
            (2, "_ZN9GNS_FRAME13CGAsyncObject13getExecutorIdEv"),
            (2, "_ZN9GNS_FRAME13CGAsyncObject13getUIThreadIdEv"),
        ],
        "primary_local_slots": [hex(0x1B43A9), hex(0x1B43AD)] + [
            hex(0x1B43B1 + 2 * i) for i in range(10)],
        "primary_local_bodies": "0020704701207047" + "7047" * 10,
        "secondary_point": "0x3d8af8",
        "secondary_offset_to_top": -0x40,
        "secondary_imports": [
            (2, "_ZThn64_N9GNS_FRAME17CGFragmentPackageD1Ev"),
            (2, "_ZThn64_N9GNS_FRAME17CGFragmentPackageD0Ev"),
        ],
    }:
        raise ValueError("host FragmentPackage virtual table differs from V21")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "elf", nargs="?", type=Path,
        default=Path(__file__).resolve().parents[1] / "reference/v21/libInstrumentPannelLarge.so",
    )
    parser.add_argument("--check-v21", action="store_true")
    parser.add_argument(
        "--host-gframe", type=Path,
        default=Path(__file__).resolve().parents[1] / "host_libs/libGFrame.so",
    )
    args = parser.parse_args()
    report = inspect(args.elf)
    report["host"] = inspect_host(args.host_gframe)
    if args.check_v21:
        check(report)
        check_host(report["host"])
        report["v21_contract"] = "PASS"
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
