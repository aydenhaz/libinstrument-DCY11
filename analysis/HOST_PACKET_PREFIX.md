# GFrame host prefix of the V21 Packet

The matching V21 `host_libs/libGFrame.so` supplies the imported
`CGFragmentPackage(CGContext*)` at `0x1b7610`. Its `C2` constructor calls
`CGPackage::CGPackage(CGContext*)` at `0x1b4434`; that constructor calls
`CGAsyncObject::CGAsyncObject()` at `0x1fd92c`. The Fragment constructor
only replaces the primary vptr and the secondary vptr at `+0x40`, using
the host table's address points `0x3d8ab0` and `0x3d8af8`.
`python3 tools/inspect_packet_factory.py --check-v21` pins the call symbols,
constructor/destructor bodies and Fragment table against the supplied host
ELF. Addresses below are virtual addresses in that image.

| Packet offset | Host construction evidence | Ownership or interpretation |
| ---: | --- | --- |
| `+0x00..+0x3f` | `CGAsyncObject` C2 at `0x1fd92c`; vptr `+0`, state byte `+4`, subobject at `+8`, two self-linked lists starting at `+0x18` and `+0x24`, additional state through `+0x3c` | A live async base with task/lock teardown, not spare bytes |
| `+0x40..+0x4b` | `CGPackage` C2 writes secondary `CGObjMem` vptr at `+0x40` and `-1` values at `+0x44/+0x48`; Fragment C2 replaces this vptr | Secondary base with destructor thunks subtracting `0x40` |
| `+0x4c..+0x63` | Package C2 initializes a self-linked record at `+0x4c/+0x50`, sentinel and zero state through `+0x60` | Owned list/atomic state; Package D2 destroys it |
| `+0x64` | Package C2 stores its `CGContext*` argument and invokes `asl::RefBase::incStrong` when nonnull | Strong reference; host destructor releases it |
| `+0x68` | Zeroed by Package C2; Package D2 reads the pointer | Host-owned pointer; its exact type is not established |
| `+0x6c..+0x83` | Package C2 calls `CGString::CGString()` on `this+0x6c`; D2 calls `CGString::~CGString()` | Constructed 24-byte string |
| `+0x84..+0x8b` | Allocates a 0x2c-byte object and stores its pointer at `+0x84`; zeroes halfword `+0x88` | Host-managed allocation and state; exact object type unknown |
| `+0x8c..+0x9f` | Package C2 calls `CGIntent::CGIntent()` on `this+0x8c`; D2 calls its D2 destructor | Constructed 20-byte intent |
| `+0xa0..+0xbf` | Package C2 writes flags, 8-byte state at `+0xa4/+0xb0`, and halfword `+0xbc` | Package state, not available to Packet private fields |
| `+0xc0..+0x167` | Original Packet factory constructs `CoverUtil` here | Separate 0xa8-byte dual-observer subobject |
| `+0x168..+0x16b` | Original factory clears the task proxy pointer | Packet's sole four-byte tail field |

The Package D2 body at `0x1b41f4` destroys the intent at `+0x8c`, string
at `+0x6c`, list/lock state and eventually the async base. It is aliased
to the matching FragmentPackage D2 export in the supplied host. The original
Packet's derived destructor calls CoverUtil D1 before entering this host
base destructor. The three host bodies are pinned by SHA256 in the
inspection gate; these offsets are additionally readable in the Thumb
stores/calls at `0x1b4434..0x1b44ba` and `0x1b4320..0x1b4378`.

The current `CGIPLPacket` class in `internal/NS_InstrumentPannelLarge/`
now has an explicitly unconstructed opaque prefix. The invalid `legacy_`
fields and all reads from them have been removed. The assembled
`onInitEnd()` holds its module in a separate singleton and uses local
activity/context variables. It stops before constructing the independent
Opening Fragment; see `analysis/OPENING_FRAGMENT_ONINIT_BOUNDARY.md`.
Do not register this class as a factory result. The host base must be
represented by genuine inherited subobjects and the Opening Fragment
must be built and owned separately.

The NDK vtable probe in `tests/packet_vtable_shape_probe.cpp` models the
base extents and vptr offsets but intentionally leaves these owning
members opaque. The prefix evidence narrows the next real declaration;
it does not establish all C++ virtual signatures or make the current
`0x16c` allocation usable at runtime.
