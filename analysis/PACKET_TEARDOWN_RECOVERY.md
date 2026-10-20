# Packet teardown corrections

The library is not replacement-ready. This checkpoint repairs observed calls;
it does not reconstruct the complete Packet or teardown method.

## Original 0x2f2f0 order

1. Emit Packet ALC trace, line 0xea.
2. Clear map-render and top-render callbacks.
3. Hide FPS with an empty CGString and size 0x18 (24).
4. Lock the host lock subobject at Packet+8 (virtual slot 2).
5. Cancel exactly one task on the Packet's CGAsyncObject base, using the
   refreshGFramePaint function address as identity.
6. Unlock Packet+8 (virtual slot 3).
7. Call CGPackage::closeAllFragments with Packet as this.
8. If MapService exists, query facility for engine 5 (slot 15); if present,
   query its state (slot 42), log it, then DestroyDisplayFacility.
9. ThirdManager::RemoveUITaskThread(device 2).
10. If Packet+0x168 proxy is nonnull, call its deleting destructor (slot 1),
    then clear that field and the Packet singleton pointer.
11. Destroy the temporary CGString.

There is no RemoveInstrumentPannelObserver call in this method. Observer
subobjects belong to Opening Fragment, whose lifetime is separate.

## Applied

Correct ALC trace, size 24, one cancellation with method identity, instance
this arguments for cancellation/fragment closure, and device-2 thread removal.
Removed invented observer removal and the invented post-load cancellation.
Original onPostLoadRes ends with primary virtual slot 6 after builder
registration; both registration and that tail dispatch are still incomplete.

The host boundary test verifies arguments and order of the applied operations.
It does not verify locks, map cleanup, proxy deletion, singleton clearing,
Android calling conventions or actual task cancellation. These are still
blockers; onDestroy must not be treated as complete.

## Construction/destruction findings

Packet factory allocates 0x16c bytes, constructs CGFragmentPackage, initializes
script state at +0xc0 (0xa8 bytes) and proxy at +0x168. Destructor 0x2eb58
destroys script state at +0xc0 then branches through a host destructor veneer.
The existing empty Packet constructor/destructor remain incomplete.

The old ScriptThis implementation is removed: it was invented from 0x30dc8,
which is actually GClassMethod equality. Its dynamic_cast compares member
method objects; it does not recover a script Packet pointer.
