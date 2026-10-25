# V21 location storage recovery — 2026-10-26

Supersedes the incomplete-type restriction in LOCATION_LAYOUT_QUARANTINE.md.
Restores real constructible CarLoc and LocInfo storage, not the Cruise query
selection/conversion flow. Both types are exposed through hsl/HSL.h.

## Recovered ARM32 layout and ownership

CarLoc is 0x1c bytes: two zero words, then the array at +8. The array has
pointer/capacity/count at +0/+4/+8 and an inline allocator at +0xc. Allocator
size is 8, with vptr at +0 and flags byte 0x11 at +4. Original inline
construction is 0x3ec2c–0x3ec40. Vtable address point 0x6cf70 has complete
and deleting destructor slots followed by allocate and release. Allocation
uses scalar operator new (0x3976e); release uses scalar operator delete
(0x39774). Array destructor 0x39746 calls release, including for null data,
only when flags bit 0x10 is set, then clears pointer/capacity/count. Copying
CarLoc is deliberately unavailable until array element/copy semantics are
recovered; default shallow copies would create double-free hazards.

LocInfo has 0x128 bytes of opaque fields, one owning dice::String16 at
+0x128, and 0x560 bytes at +0x12c; 8-byte alignment yields size 0x690.
Host copy routine 0x38f518 in libhsl copies those two byte ranges and assigns
the string. The recovered implementation therefore uses real String16
construction, deep copying, assignment and destruction. Instrument constructor
0x394ec supplies exact initialization ranges, including 1, -1, -1.0f,
-1.0 double high words, and 0xff sentinel bytes. Padding is preserved rather
than blanket-zeroed. Unknown field meanings remain unnamed opaque bytes.

## Validation

`tests/location_types_original_test.py` compiles the native implementation
with AddressSanitizer and UBSan, then compares its serialized ARM layout
against executed original Thumb constructor instructions for initial fills
0x00, 0xa5 and 0xff. Only the string pointer address is normalized; the
external String16 constructor is mocked in the emulator. All remaining
0x690 bytes, including padding, are compared and guard regions checked.
The native side exercises empty strings, deep copy/assignment, owning array
allocation/release, and borrowed arrays. The test also executes the original
CarLoc initializer and array destructor (owned, borrowed and null cases),
and verifies original allocator slots and scalar allocation symbols.

This environment requires `ASAN_OPTIONS=detect_leaks=0` because LeakSanitizer
cannot inspect /proc tasks. Address/undefined-behavior checks pass; this is
not a LeakSanitizer result. Native and NDK r26c syntax/layout/API probes pass.
The six-case original Cruise observer/engine regression also passes.

## Remaining integration

The original map-present branch at 0x3ec22 invokes selector 0x417f8.
Result 1 calls GetCarPosition with engine 1; otherwise it obtains Pos,
constructs LocInfo and converts the returned fields into CarLoc array
entries before map use. The previously removed unconditional, unused host
calls are not that flow and remain removed. Recover the selector, conversion,
array growth and map-consumption sequence together before reattaching it.
Packet registration and complete Fragment construction/lifecycle remain
unresolved; no device validation or replacement-ready SO is claimed.
