# External-screen UI observer operations — 2026-10-26

Recovered six callable ARM32 operations in internal/abi/ExscreenUiObservers.cpp.
They operate on a genuinely constructed V21 AutoExscreenNotifyObserver;
they do not construct that object or fabricate its lock/allocator vtables.

| Reconstructed operation | Original address | Storage |
| --- | --- | --- |
| addMapRenderObserver | 0x2d55c | lock +0x24, array +0x34 |
| removeMapRenderObserver | 0x2d6f8 | same |
| addUiUpdateObserver | 0x2d7b0 | lock +0x48, array +0x58 |
| removeUiUpdateObserver | 0x2d964 | same |
| setProjectionSizeListener | 0x2deac | lock +0xb4, listener +0xc4 |
| setUiStyleListener | 0x2df28 | lock +0xb4, listener +0xc8 |

Each lock uses virtual slots +8/+12 for lock/unlock. Every operation locks,
even null or duplicate requests. Add ignores null and existing pointers;
remove erases the first matching pointer and shifts remaining entries without
clearing the now-unused trailing word. Setter operations store the supplied
pointer, including null, without taking ownership or invoking the listener.

Arrays contain data/capacity/count at +0/+4/+8, with inline allocator at +12
and flag byte +16. Growth matches the recovered original: count+1, plus 5
when old capacity <5, count when 5..499, or count/4 when >=500, if the flags'
low nibble equals 1. Other modes use count+1. Allocation/release use allocator
slots +8/+12; existing pointer entries are copied, then old storage is freed.
Overflow is rejected before allocation-size wrap. A scoped lock also unlocks
on C++ exceptions, avoiding a stuck lock if allocation fails.

## Validation

`tests/exscreen_ui_observers_original_test.py --so <fresh ARM SO> --nm <llvm-nm>`
executes both original Thumb functions and newly compiled ARM implementations.
Thirty scenarios (656 operations per binary) compare lock/allocation/release
traces and complete owner/heap bytes after every operation. Coverage includes
null and duplicate adds, missing/repeated removal, order-preserving removal,
empty lists, growth across 5 and 500 capacity thresholds, both allocator modes,
60-element growth/removal sequences, listener replacement and clearing.
Host allocation/locks and logging are mocked; no device concurrency result
is claimed. Diagnostics are omitted from reconstructed methods.

Original-vs-rebuilt Cruise view bindings (16 cases), map call ABI checks,
scene/VFP tests and owning-location initialization/release tests also pass.
NDK r26c archive hash reverified; production overlay compiles 57/57 units and
passes --no-undefined linking. Development SO SHA256:
cd2b3ea186656ed127b7a1f859d208b03fa412bdaba58e96f42bc072d9e29c44.

## Integration boundary and broadcast evidence

These operations are not yet wired into Cruise lifecycle. Original creation
obtains the UI observer owner separately for each operation through 0x296a0,
using Fragment+0x34c as the selector, then registers embedded targets at
+0x390/+0x394/+0x398/+0x39c. Destruction removes the first two and clears the
last two. Getter 0x296a0 uses guarded singleton storage (selectors 1, 2 and
fallback) and constructor 0x29798. That construction, including real lock,
allocator and observer vtables, must be restored before attachment is safe.

Broadcast construction is a separate blocker. Original 0x41240 builds the
20-byte Callable plus a 16-byte wrapper and a 36-byte shared invoker/control
allocation. Register 0x15084 copies its wrapper/shared state; remove 0x166dc
searches by callable identity and then releases the stored copy. Existing
isolated VoidCallable models are not proof of ABI-compatible host ownership.
No broadcast registration was added with those models. Full singleton and
Fragment construction, broadcast identity/ownership and Packet registration
remain required before a replacement-ready library; no device test is claimed.
