# Packet preload recovery — 2026-10-24

Original `0x2f464` registers the top-render callback, locks Packet+8,
binds refreshGFramePaint and calls CGAsyncObject::scheduleTaskHelper with
1000, 0, -1, the method identity and source token line 0x110. It releases
the local Callable before unlocking. The original allocations are 0x10
and 0x24-byte closure internals, not independent 0x16/0x36-byte objects.

The root and assembled lifecycle implementations now use the existing GPI
Callable adapter for this flow, replacing the leaked placeholder buffers.
The trace uses the original line 0x10e. Evidence is in
analysis/v21_verified/packet_preload.txt.

The assembled native ASan/UBSan contract test verifies callback identity,
lock scope, scheduling arguments, retained callback invocation, and exception
unlocking. This does not validate host-side Callable cloning or ownership on
Android. No new Android full build or device test was performed.

Packet real inheritance/vtables/RTTI, post-load Fragment builder registration,
Opening construction and full Fragment lifecycle remain incomplete. Automatic
GFrame package registration remains disabled. This is a functional preload
recovery, not a usable replacement library.
