# Original V21 Packet factory runtime contract

The input is `reference/v21/libInstrumentPannelLarge.so` (ELF32 ARMv7).
Run `python3 tools/inspect_packet_factory.py --check-v21` from the repository
root to re-read the original ELF's initializer array, factory bytes, four
virtual table groups, and the matching host GFrame constructor/relocations.
The check passed on the committed V21 binaries. It validates static
evidence, not a rebuilt factory or Android behavior.

## Creation order

| Factory site | Operation | Object address or target |
| --- | --- | --- |
| `0x2eb0e..0x2eb12` | `operator new(0x16c)` | Packet allocation |
| `0x2eb1a` | Imported `CGFragmentPackage(CGContext*)` | Packet `+0` |
| `0x2eb20` | Local `CoverUtil` constructor `0x17cac` | Packet `+0xc0` |
| `0x2eb26` | Clear UI task proxy | Packet `+0x168` |
| `0x2eb36..0x2eb42` | Replace base virtual pointers with Packet pointers | Packet `+0xc0`, `+0xc4`, `+0x40`, `+0` |
| `0x2eb48` | Publish Packet singleton | DSO BSS RVA `0x6fd48` |
| `0x2eb4a` | Return pointer | Host `CGPackage*` factory result |

The singleton target follows from literal `0x411fe` at `0x2eb54`,
added to Thumb PC `0x2eb4a` by the instruction at `0x2eb46`.
It is the **same** `0x6fd48` slot used by LargeScreenImpl screen-mode
branches at `0x2e20e`, `0x2e2fa`, `0x2e36e`, `0x2e3ea`, and another
package receiver at `0x158aa`. These calls receive the constructed Packet's
host package base; they must not pass `LargeScreenImpl::this`.

## Host base construction boundary

The matching `host_libs/libGFrame.so` has
`CGFragmentPackage(CGContext*)` at `0x1b7610`. Its call at `0x1b7614`
targets PLT `0x11098c`, whose relocation names
`CGPackage::CGPackage(CGContext*)`. The returned host object gets its
`CGFragmentPackage` secondary vptr at `+0x40` and primary vptr at
`+0`. The vtable pointer comes through GOT `0x3e9c04`, whose relocation
names `_ZTVN9GNS_FRAME17CGFragmentPackageE`. Its constructor stores
`vtable+0x50` at `+0x40`, then `vtable+8` at `+0`.
The local Packet factory overwrites both with derived Packet vptrs
after constructing CoverUtil. This is why a zeroed `0xc0`-byte prefix
cannot stand in for the host base.

## Derived virtual table groups

| Packet subobject | Offset | Vtable address point | Offset-to-top | Visible functions |
| --- | ---: | ---: | ---: | --- |
| `CGFragmentPackage` | `+0` | `0x6b800` | 0 | 16 slots; destructors 0/1, `onInitEnd` 6, `onDestroy` 9, `onPreLoad` 10, `onPostLoadRes` 14, `onPostAlreadyLoadRes` 15 |
| host `CGObjMem` | `+0x40` | `0x6b848` | `-0x40` | 2 slots |
| `CoverUtil` / `MapSurfaceViewObserver` | `+0xc0` | `0x6b858` | `-0xc0` | 4 slots |
| `CoverUtil` / `IBLMapBusinessDataObserver` | `+0xc4` | `0x6b870` | `-0xc4` | 3 slots |

All four vtable headers reference Packet RTTI `0x6b87c`. Most function
pointers have `R_ARM_RELATIVE` relocations; two imported
`CGAsyncObject` entries in the primary table use `R_ARM_ABS32`.
`0x2eb7e` and `0x2eb82` are four-byte virtual bodies that return
`0` and `1` respectively (primary slots 4 and 5). Primary slots 7 and 8
at `0x2f2ec` and `0x2f2ee`, and slots 11 through 13 at
`0x2f5f4`, `0x2f5f6`, `0x2f5f8`, each contain only `bx lr`.
Their source-level return types and names cannot be inferred from that
instruction alone. Slot 9 starts the nontrivial `onDestroy` body at
`0x2f2f0`; it must not be treated as another two-byte return.
`0x16c = 0xc0 + 0xa8 + 4`: the host base, CoverUtil, and proxy occupy
the full allocation. Merely matching its total size cannot reproduce
constructors, RTTI, virtual methods, lock ownership, or teardown.

The current `CGIPLPacket` class is only a partial storage reconstruction.
Its constructor no longer publishes its own pointer; only a future complete
factory may invoke its private publication step. The shutdown contract test
simulates that step after installing a mock host lock. The registration
helper deliberately remains disconnected.
The safe recovery sequence is to model the host base and inherited virtual
ABI, implement the CoverUtil/Packet overrides and lifetime, make the factory
publish only at the end, then enable
registration and run on-device validation. No operational Packet factory is
claimed at this checkpoint.

The separate unpublished construction guard in
`internal/abi/PacketFactoryStages.h` now covers allocation, host C2,
CoverUtil placement construction, proxy initialization, and rollback. It
does not transfer ownership or install derived virtual pointers. See
`analysis/PACKET_FACTORY_STAGING.md`.

The original Packet destructor calls CoverUtil D1 at `0x17dfc` before
branching to the host base destructor. CoverUtil D1 destroys the nested
CGString at Cover `+0x44`, then local String16 at Cover `+0x18` (after
optional logging); it does not itself release the image or unregister an
observer. `tools/inspect_packet_factory.py --check-v21` now pins the
destructor body, imported CGString D1, local String16 D1 and Packet call.
