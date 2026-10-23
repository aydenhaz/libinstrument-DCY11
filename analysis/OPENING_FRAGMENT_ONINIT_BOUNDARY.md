# Packet onInitEnd Opening Fragment ownership boundary (V21 ARM32)

The original `CGIPLPacket::onInitEnd` at `0x2eb88` does not store an Opening
Fragment, activity pointer, mode, or `ObjectRuntimeInfo` in Packet fields.
Packet's allocation is `0x16c` bytes and ends with the proxy at `+0x168`.
The discarded `legacy_` fields overlapped its live host package vptr, lock,
lists, context, strings and CoverUtil subobject. Reading them during init
would interpret those owned objects as unrelated pointers and an integer.

| V21 site | Receiver or value | Meaning |
| --- | --- | --- |
| `0x2ecc6..0x2ecd6` | Host simulation, then Packet in `r8` | Get FragmentActivity and Packet package context |
| `0x2ecdc..0x2ecf8` | Activity in `fp` | Dispatch two activity virtual slots and test `activity+8`; fatal branch starts at `0x2f220` |
| `0x2ecfc..0x2ed0a` | `new(0x440)` into `r6`; call `0x41954` | Construct a distinct Opening Fragment tagged `HMI_OPENING` |
| `0x41968..0x419d6` | Opening Fragment | Construct host `CGFragment`, `hsl::ObserverBase`, store engine 5 at `+0x308`, initialize screen/module fields at `+0x34c/+0x350` |
| `0x2ed3e..0x2ed5c` | Opening Fragment | Install observer vptrs and construct `bl::ObjectRuntimeInfo` at `+0x384`, passing its `+0x380` BLObject subobject |
| `0x2ee82..0x2eea0` | Opening Fragment `+0x308`, `+0x34c`, `+0x3a0` | Derive timer ID and load/configure the fragment surface; no constant timer ID of zero |
| `0x2f17e..0x2f1a0` | Opening Fragment in `r6`; activity in `fp` | Set fragment context/type and pass it to `CGActivity::setFragment` with a constructed `CGIntent` |
| `0x2f1ba..0x2f1ca` | Fragment in `r0`; Packet `+0xc0` in `r1` | Tail call `AutoOpeningFragment::setCoverUtil(0x3bdbc)` |

The 7.5 V21 ELF gate `python3 tools/inspect_packet_factory.py --check-v21`
now verifies the allocation immediate, constructor/tag, imported host base,
embedded runtime-info constructor, activity wiring and Cover binding tail.
The source `CGIPLPacket::onInitEnd` stops after its verified module/thread and
host context prefix. It no longer reads fake Packet fields, allocates a
separate heap `ObjectRuntimeInfo`, inserts timer ID zero, or publishes a
screen mode from uninitialized memory. The class reserves its entire prefix
as explicitly **unconstructed** storage. No factory is registered for it.

The Opening Fragment's complete base hierarchy, observer interfaces,
constructor, error path and ownership remain to be rebuilt before restoring
the rest of `onInitEnd` or activating the Packet factory. The linked SO
therefore remains a development artifact and cannot replace the V21 image.

The isolated `PendingOpeningHost` now calls the matching host
`CGFragment` C2 with launch mode 0, constructs `hsl::ObserverBase` at
`+0x2e8` with the inline name `AutoBaseFragment`, and writes engine 5 at
`+0x308`. It always rolls back through the imported `CGFragment` D2; it
does not install the derived Fragment vptrs or leave a live Fragment. The
Activity calls at vtable slots 12 and 25 followed by the `activity+8` gate
are modeled separately. Details: `analysis/OPENING_FRAGMENT_STAGING.md`.
