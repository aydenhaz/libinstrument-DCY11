# Cruise location conversion and append — 2026-10-26

Restored `CarLocationEntry::assignLocInfoRecord` and
`CarLocationArray::append` in the existing location ABI implementation.
These are callable conversion/storage primitives; the incomplete Fragment
view flow does not invoke them yet.

## Entry conversion

Original onCreateView 0x3eca8–0x3ed2c reads one LocInfo record (stride 0x110)
and produces one 0x68-byte CarLoc array entry:

| Entry offset | Record offset / initialization |
| --- | --- |
| 0x00..0x0b | copy 0x88..0x93 |
| 0x0c..0x13 | copy 0x98..0x9f |
| 0x14..0x17 | copy 0x98..0x9b again |
| 0x18..0x27 | copy 0x00..0x0f (two doubles) |
| 0x28..0x2b | copy 0x18..0x1b |
| 0x30..0x48 | zero; if signed word at record+0x58 >= 1, copy 24 bytes from record+0x20 and set entry+0x48 to 1 |
| 0x4c..0x57 | three 0xffffffff words |
| 0x58..0x67 | zero |

Padding +0x2c..0x2f and +0x49..0x4b remains untouched, matching the original.
The API requires a readable 0x110-byte source record. Field meanings have
not been guessed from their offsets.

## Array append

Original 0x39660 copies 0x68 bytes per element. If count+1 exceeds capacity,
it snapshots the input first because it can point into the old allocation.
For allocator flags low nibble 1, capacity becomes count+1 plus:

- 5 when old capacity < 5;
- count when old capacity is 5..499;
- count/4 when old capacity >= 500.

Other growth modes allocate count+1. Existing elements are copied, old
storage is released with scalar delete, then the input snapshot is appended.
The original growth path releases the old allocation unconditionally; a
borrowed external buffer must not be grown with this allocator. Destructor
ownership still follows flag 0x10, as recovered previously. The reconstruction
adds overflow rejection before 32-bit allocation size arithmetic wraps.

## Scene selection evidence for the next integration

Original `AutoCruiseFragment::GetNaviType()` is 0x417f8:
GetGuideHandle -> guide virtual slot +0x20 -> service. Missing guide/service
returns 2. Service slot +0xac returning nonzero returns 0; otherwise slot
+0xc4 returning nonzero returns 1, else 2. Diagnostics do not change that
result. Cruise uses GetCarPosition(engine 1) only for result 1; all other
results take the Pos/GetLocInfo path. LocInfo count is at +0x4b0; records
start +0x180. The float at +0x10 is converted to the first CarLoc word.
These observations are evidence, not a newly wired scene implementation.

## Validation and limits

`ASAN_OPTIONS=detect_leaks=0 python tests/location_conversion_original_test.py`
executes original Thumb conversion for signed validity -1/0/1/2, including
VFP loads/stores, and compares all 0x68 bytes against the reconstructed code.
It also executes 1100 original appends for each flag mode 0x11/0x10, compares
capacity checkpoints and verifies all retained element bytes. Native
ASan/UBSan checks cover the same sequences, source aliasing during growth,
and oversized-count rejection. Original owning-type initialization/release,
ARM32 layout/API probes and Cruise observer/engine regressions still pass.
LeakSanitizer is disabled due to this environment's /proc task restriction.

Full view integration still needs scene dispatch, complete record iteration,
CarLoc speed conversion, map calls and lifetime cleanup together. Packet
factory/registration and complete Fragment lifecycle remain unresolved. No
new device runtime test or usable replacement SO is claimed.
