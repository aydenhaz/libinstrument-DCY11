# Broadcast Publisher state resolution and disposal — 2026-10-26

Corrected the prior resolver interpretation. Original 0x15258 invokes slot 0
of the broadcaster to obtain the Publisher key/object, then calls veneer
0x45584 -> PLT 0x13bb4. The resolved symbol is:
`_ZN3GPI9Broadcast8internal9Publisher3getEPvPFS3_vEPFvS3_E`.
Arguments are the returned pointer, a state factory, and a state disposer.
There is no RTTI cast in this resolver. Callback addresses are original
0x15370 and 0x15386; the implementation supplies rebuilt function addresses.

Added BroadcastPublisherHost.cpp with the exact imported host ABI and
resolveBroadcastList. It delegates state lookup/lifetime ownership to the
host Publisher; it does not use a guessed offset into CGBroadcaster, create
a parallel global registry or fabricate type-info identities.

createBroadcastListState allocates 16 bytes on ARM32, initializes both list
links to the sentinel, sets count zero and constructs std::mutex at +12.
destroyBroadcastListState accepts null. For a live state it first destroys
the mutex, then deletes all remaining nodes backwards, then frees state
storage without destroying its mutex twice. The Publisher must have stopped
all concurrent users before disposal, as required by the original sequence.

Validation:
- broadcast_publisher_original_test.py resolves the actual veneer/PLT symbol
  and executes original and freshly built ARM resolver code with a host-get
  mock. It checks slot-0 receiver, returned key, both callback pointers and
  returned state pointer.
- It invokes the actual original/rebuilt factory/disposer callbacks for
  zero, one and three nodes, compares initialization and destruction order,
  tracks all node/wrapper/state frees, and checks null disposal. Wrappers
  have inert shared state in this test; this is not full shared-control ABI
  certification.
- Native ASan/UBSan tests now also dispose nonempty and empty Publisher states,
  preserving external callable copies, in addition to earlier concurrency
  and duplicate/removal cases. Leak detection remains disabled due to the
  documented /proc restriction.
- Original list semantics (11 steps), broadcast dispatch (18 cases) and
  Cruise view-binding regressions (16 cases) still pass.
- NDK r26c: 60/60 compiled, --no-undefined link PASS. Development SO SHA256:
  575ab0dbb22b0b9d54d900ab4870f71a16a0fd095adfe4f93eb59e8da89ecd1e.

Broadcast lifecycle attachment still waits for original/rebuilt shared-control
interoperability and valid Fragment callback vtables. Adapter RTTI compatibility
with original method objects is a separate issue from this corrected resolver.
External-screen singleton/timer construction and Packet registration remain
incomplete. No device validation or usable replacement is claimed.
