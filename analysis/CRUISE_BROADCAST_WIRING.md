# Cruise broadcast attachment and shared-control validation — 2026-10-26

The two broadcast subscriptions are now attached after the host-observer tail
in both Cruise onCreateView implementations, and removed before host-observer
cleanup/base destruction in onDestroyView. IDs are 0x10 and 0x10004 (the
latter is pinned by original literals at 0x3f050/0x3f518). Their member slots
are 0xe4 and 0xec respectively. Each operation obtains CGBroadcaster, resolves
its actual Publisher state, reads channel at broadcaster+4, and applies the
recovered list registration/removal. Temporary Callables release ownership
after the list has retained its own copy.

## Shared-control interop test

broadcast_shared_interop_test.py loads three ARM ELF images into Unicorn:
the original V21 instrument, the rebuilt library and NDK r26c libc++_shared.
It applies ARM dynamic relocations and executes real rebuilt factory/wrapper/
method code plus real libc++ RTTI and shared-reference release. Only external
heap/OS/string primitives are mocked; reference count, RTTI comparison and
Callable ownership are not replaced by fake success functions.

The original copy routine 0x152b2 clones a rebuilt wrapper and retains its
actual control block. Original equality 0x151fe matches a rebuilt peer of the
same target/slot and rejects the other slot through real dynamic_cast code.
Original destructor 0x15134 destroys the registration source; its destination
still dispatches the original message reference. Destroying all objects
frees exactly seven allocations once, leaving no live allocation.

The test then executes the actual rebuilt attach/duplicate-attach/message
invocation/detach/repeated-detach sequence. Host getInstance/Publisher lookup
and OS mutex operations are mocked, while list nodes, clones, equality,
reference counting and destruction run real ARM code. Both channels retain
one node after duplicate registration, dispatch correctly, and finish empty;
Publisher state disposal frees every tracked allocation. This validates the
observed control-block ABI with NDK r26c runtime, not a device run against
the APK's installed libc++ binary.

Existing lifecycle-entry test now checks broadcast attachment/removal order,
with the separately tested binding functions intercepted. All 16 cases pass.
Publisher callback cleanup and 18 member-dispatch cases also pass. NDK r26c:
61/61 compiled, strict --no-undefined link PASS. Development SO SHA256:
59bcaa26d87db7167a27c21304d52ba840a04c0f2f6a963bad829b3b43e9a81e.

## Unfinished requirements

This resolves the broadcast attachment blocker. It does not complete the
external-screen singleton or Packet factory. Singleton 0x29798 still requires
its five actual locks, owning ObserverBase/AsyncObject, 0x90-byte timer and
its separate closure, plus corresponding destruction and event callbacks.
Packet's storage-only class still lacks genuine derived virtual subobjects
and a fully constructed Opening Fragment. Registering that current storage
would reintroduce the known invalid construction/black-screen path. Packet
registration remains disabled until those lifetime requirements are restored.
The now-wired Cruise methods also require a genuinely constructed Fragment
with valid callback slots; exposing an unconstructed object remains forbidden.
