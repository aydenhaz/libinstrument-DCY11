# V21 Packet virtual dispatch boundary (2026-10-23)

The original Packet table at `0x6b7f8` derives from the matching
`libGFrame.so` `CGFragmentPackage` table at `0x3d8aa8`. Its primary address
point has 16 entries. The host constructor installs the host primary address
point at Packet `+0` and the two-entry `CGObjMem` destructor address point
at `+0x40`; the Packet factory later overwrites both, as well as CoverUtil's
two observer pointers. `python3 tools/inspect_packet_factory.py --check-v21`
verifies both ELFs and the six Packet destructor adjustor thunks.

| Primary slot | Host | Packet | Current signature evidence |
| ---: | --- | --- | --- |
| 0–1 | CGPackage/FragmentPackage destructor | Packet D1/D0 | destructor pair |
| 2–3 | CGAsyncObject executor/UI thread getters | same imported functions | imported symbol names |
| 4–5 | return 0 / return 1 | return 0 / return 1 | return types not established |
| 6 | `bx lr` | `onInitEnd` | Packet string cross-reference |
| 7–8 | `bx lr` | `bx lr` | return types and names unknown |
| 9 | `bx lr` | `onDestroy` | Packet string cross-reference |
| 10 | `bx lr` | `onPreLoad` | Packet string cross-reference |
| 11–13 | `bx lr` | `bx lr` | return types and names unknown |
| 14 | `bx lr` | `onPostLoadRes` | Packet string cross-reference |
| 15 | `bx lr` | `onPostAlreadyLoadRes` | Packet string cross-reference |

The host's `+0x40` `CGObjMem` table has offset-to-top `-0x40` and two
imported destructor thunks. Packet replaces them with `0x2fca0` and
`0x2fca6`, each subtracting `0x40` from `this` before branching to Packet
D1/D0. The Packet CoverUtil map observer at `+0xc0` keeps the CoverUtil
callbacks but replaces destructor slots with `0x2fcac`/`0x2fcb2`, which
subtract `0xc0`. The business observer at `+0xc4` uses `0x2fcb8`/`0x2fcbe`
to subtract `0xc4`. Its callback remains CoverUtil's `0x18068` thunk, which
subtracts 4 from the business observer pointer before calling the CoverUtil
method at `0x17e9c`.

These relationships constrain a future C++ host base declaration and derived
Packet class. The present `CGIPLPacket` is a non-polymorphic, partial storage
type. Its `0x16c` size assertion does not supply these vptrs, inherited
virtual signatures, RTTI, or host-owned member lifetime. The unpublished
construction guard therefore still destroys its object without registration
or ownership transfer. The owning host prefix and the conflicting current
Packet fields are detailed in `analysis/HOST_PACKET_PREFIX.md`. No
installable replacement follows from this table.

`tests/packet_vtable_shape_probe.cpp` is a freestanding 32-bit C++ hierarchy
with the same base extents, virtual-slot counts and override positions.
`python3 tests/packet_vtable_shape_test.py` compiles it with `g++ -m32`, then
checks the generated relocation slots, four address points (`+8/+0x50/+0x60/
+0x78`), offset-to-top words (`0/-0x40/-0xc0/-0xc4`), destructor thunks,
Cover business callback thunk and RTTI base offsets (`0xc002`/`0x402`). This
probe also passes with `--arm`, using `arm-linux-gnueabihf-g++` with ARMv7,
Thumb and softfp flags. Its ARM ELF32 object has 29 Packet table relocations
at the same offsets. The Android NDK r26c probe also passes with
`python3 tests/packet_vtable_shape_test.py --ndk /path/to/android-ndk-r26c`:
it checks the same 29 relocations and four address points in an Android
ARMv7 Thumb softfp object. Clang selects the D2 alias for the primary
non-deleting destructor slot, where GNU GCC selects D1. This establishes
the inherited table **shape** under both toolchains. The dummy method names
and opaque storage do not reconstruct GFrame. The complete project now
compiles and strictly links with Android NDK r26c (see
`analysis/ANDROID_NDK_R26C_BUILD.md`), but the real Packet base declaration,
signatures, ownership and device behavior still need recovery.
