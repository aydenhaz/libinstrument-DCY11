# Packet's second base is CoverUtil

This corrects the earlier script-binding hypothesis. The original ELF RTTI is
unambiguous; see v21_verified/packet_coverutil_rtti.json and tools/inspect_rtti.py.

| Type | Base | Offset | Inheritance |
| --- | --- | --- | --- |
| CGIPLPacket | GNS_FRAME::CGFragmentPackage | 0 | public, nonvirtual |
| CGIPLPacket | NS_MultiScreenBase::CoverUtil | 0xc0 | public, nonvirtual |
| CoverUtil | hsl::MapSurfaceViewObserver | 0 | public, nonvirtual |
| CoverUtil | bl::IBLMapBusinessDataObserver | 4 | public, nonvirtual |

RTTI roots are 0x6b87c (Packet) and 0x69144 (CoverUtil). Base offset/flag words
are 0x0000c002 for CoverUtil in Packet and 0x00000402 for the second observer
in CoverUtil. Imported base RTTI is resolved through ELF relocation symbols.

Consequences:

- Local constructor 0x17cac initializes CoverUtil, not script state.
- Packet's secondary observer vptrs at +0xc0/+0xc4 follow these actual bases.
- CoverUtil's allocation extent is 0xa8 up to Packet's proxy field at +0x168.
- A valid Packet requires these observer interfaces and their real callbacks.
  Padding and a correct total size cannot supply them.

The existing Packet is still a partial storage reconstruction. Its constructor
does not construct the host/CoverUtil bases. The earlier version prematurely
published the rebuilt singleton; this was removed after the device failure.
Factory registration must remain disabled until those bases are recovered.

## SurfaceConfig defaults

CoverUtil constructor calls local 0x16302 for SurfaceConfig at CoverUtil+0x20.
That constructor builds CGString at SurfaceConfig+0x24 and initializes:

| Offset | Value |
| --- | --- |
| 0x1c | 0xffffffff (-1) |
| 0x20 | 8 |
| 0x48 | 1 |

Other observed scalar/flag stores are zero. Padding is not a semantic field.
The source formerly zeroed every field; it now restores these three defaults.
No semantic names are assigned to these slots without further dataflow evidence.

The poisoned-storage host test verifies defaults and exactly one string
construction/destruction. ASan/UBSan pass with LeakSanitizer disabled. Compiling
the test for ARM EABI verifies the existing size/offset assertions. This is not
an Android runtime or JSON consumption test.
