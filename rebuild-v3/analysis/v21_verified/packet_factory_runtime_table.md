# Original V21 Packet factory runtime contract

The input is `reference/v21/libInstrumentPannelLarge.so` (ELF32 ARMv7).
Run `python3 tools/inspect_packet_factory.py --check-v21` from the repository
root to re-read the original ELF's initializer array, factory bytes, four
virtual table groups, and relocations. The check passed on the committed V21
binary. It validates static evidence, not a rebuilt factory or Android behavior.

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

## Derived virtual table groups

| Packet subobject | Offset | Vtable address point | Offset-to-top | Visible functions |
| --- | ---: | ---: | ---: | --- |
| `CGFragmentPackage` | `+0` | `0x6b800` | 0 | 16 slots; destructors 0/1, `onInitEnd` 6, `onPreLoad` 10, `onPostLoadRes` 14, `onPostAlreadyLoadRes` 15 |
| host `CGObjMem` | `+0x40` | `0x6b848` | `-0x40` | 2 slots |
| `CoverUtil` / `MapSurfaceViewObserver` | `+0xc0` | `0x6b858` | `-0xc0` | 4 slots |
| `CoverUtil` / `IBLMapBusinessDataObserver` | `+0xc4` | `0x6b870` | `-0xc4` | 3 slots |

All four vtable headers reference Packet RTTI `0x6b87c`. Most function
pointers have `R_ARM_RELATIVE` relocations; two imported
`CGAsyncObject` entries in the primary table use `R_ARM_ABS32`.
`0x16c = 0xc0 + 0xa8 + 4`: the host base, CoverUtil, and proxy occupy
the full allocation. Merely matching its total size cannot reproduce
constructors, RTTI, virtual methods, lock ownership, or teardown.

The current `CGIPLPacket` class is only a partial storage reconstruction:
its constructor publishes its own pointer before any equivalent base
construction. The registration helper deliberately remains disconnected.
The safe recovery sequence is to model the host base and inherited virtual
ABI, implement the CoverUtil/Packet overrides and lifetime, replace premature
singleton publication with the factory's final store, then enable
registration and run on-device validation. No operational Packet factory is
claimed at this checkpoint.
