# Broadcast list registration and removal — 2026-10-26

Implemented addBroadcastCallable/removeBroadcastCallable over the recovered
broadcast list state. This is the internal list state, not the public
CGBroadcaster pointer. Its sentinel and std::mutex must already be constructed.

## Original contract

Original registration 0x15084 and removal 0x166dc obtain list state through
0x15258 and take the channel word from broadcaster+4. List state has next/prev
sentinel links at +0/+4, count at +8 and libc++ std::mutex at +12. A node is
32 bytes: links +0/+4, 20-byte Callable at +8, channel at +28. ARM build
assertions pin every offset/extent used by the implementation.

Search 0x1527c walks previous links, comparing callable identity before the
channel. Registering an existing identity/channel copies the new callable
into that node; otherwise a new node is inserted immediately after the
sentinel and count increases. Different channels and different member/target
identities remain separate. Registration never invokes the callback.

Removal first copies the matching callable into a local temporary, unlinks
and deletes its node under the lock, unlocks, then destroys the retained
copy. The reconstruction preserves this final-release ordering to avoid
running final shared-method destruction while the list mutex is held.
Missing or repeated removal leaves the list unchanged. std::lock_guard also
releases the lock when C++ allocation/copy throws. This uses the actual
libc++ mutex interface, not a guessed byte buffer or a substituted lock type.

## Validation

`tests/broadcast_list_original_test.py` executes original ARM registration,
search, assignment, unlink and cleanup instructions for 11 add/remove
operations. Callable identity, clone/destruction and C++ mutex runtime calls
are mocked. It checks full link order, bidirectional consistency, count,
channel separation, duplicate updates, missing/repeated removal, balanced
wrapper/node allocations and final temporary release after unlock.

`tests/broadcast_list_test.cpp` tests the reconstructed list with the real
new BroadcastCallable implementation under ASan/UBSan: duplicate identity,
channel/member/target separation, temporary registration lifetime, missing/
repeated removal and complete sentinel restoration. Four concurrent threads
each perform 1000 add/duplicate-add/remove cycles and finish with an empty,
consistent list. This is a stress test, not a ThreadSanitizer proof.
ASan uses detect_leaks=0 for the existing environment limitation.

## Remaining integration

This implements list operations but does not yet connect them to Cruise.
Correction: resolver 0x15258 calls broadcaster virtual slot 0, then host
GPI::Broadcast::internal::Publisher::get with create/dispose callbacks. It
does not perform an RTTI cast. This resolver and those callbacks are now
implemented and ARM-tested; see BROADCAST_PUBLISHER_RECOVERY.md. Original
shared-control-block interactions with rebuilt method objects still require
end-to-end verification. The ARM reference test intentionally mocks
Callable shared ownership and must not be described as an ABI interoperability
proof. Adapter RTTI does not claim to match original method RTTI.

Real Fragment target vtables, external-screen singleton construction/timer,
and Packet registration remain incomplete. No device readiness is claimed.
