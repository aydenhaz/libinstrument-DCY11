# Cruise message-reference Callable — 2026-10-26

## Correct signature recovered from RTTI

The original Callable vtable address point 0x68f5c references RTTI spelling:
`N3GPI7Closure8CallableIFvRKN9GNS_FRAME16STGBroadcastMsg_EEEE`.
It is `void(const GNS_FRAME::STGBroadcastMsg_&)`, not `void()`.
Wrapper RTTI at 0x6df34 and method RTTI at 0x6defc agree. The message remains
opaque in production; no fabricated message struct layout is supplied.

Added this exact Callable specialization in GFrame/GPIBroadcastCallable.h,
with a factory for the two original Cruise member descriptors (0xe4,1) and
(0xec,1). These are virtual byte offsets, not addresses into the old binary.
ARM member descriptors encode signed this-adjustment in the high bits of the
second word and virtual dispatch in its low bit. The message reference is
forwarded unchanged to the adjusted receiver's function. Null member equality
matches the original ARM rule; a null function must not be invoked.

## Ownership and interface shape

The callable owns a cloned wrapper plus a shared method object. Copy clones
the wrapper and retains the method; move transfers ownership; reset releases
both. The wrapper also contains the target and member descriptor, so a clone
never depends on the source wrapper's storage. Listener targets themselves
are not retained. Destruction releases method ownership then the wrapper.
Constructor ordering releases the method automatically if wrapper allocation
throws. Native ASan/UBSan tests cover copy, move, assignment/self-assignment,
reset, clone-after-source-destruction, identity mismatch and adjusted calls.
LeakSanitizer is disabled for the documented /proc inspection restriction.

ARM32 compile-time extents: member descriptor 8, wrapper 16, method 24,
Callable 20 bytes. Callable slots are D1/D0/empty/reset/equals/invoker;
method slots are equals/D1/D0/targetAddress/methodAddress/invoke; wrapper
slots are D1/D0/invoke/clone. Adapter method RTTI is private to the rebuilt
implementation: it does not claim equality with an original-library method
object. Both registration and removal must use the same adapter identity.

## Validation and integration boundary

broadcast_callable_original_test.py verifies original RTTI and executes
original wrapper 0x41444 and the freshly compiled wrapper for 18 direct,
virtual and positive/negative-adjustment cases. Each checks receiver and
message-reference registers and preserves message bytes. This proves dispatch
and argument handling, not complete original broadcaster ownership interop.
Native tests additionally verify matching/mismatching identities and that the
factory retains distinct original 0xe4/0xec slots. Production ARM build passes
all four size assertions. Existing 30 UI observer scenarios and 16 Cruise
view-binding cases still pass.

Broadcast registration/removal is NOT yet attached. Original list routines
0x15084/0x166dc obtain shared list state via 0x15258, compare callback identity,
copy wrappers and retain/release shared-control state. Full original-list
copy/search/destruction interop and real target vtables must be verified
before wiring this adapter to that list. No no-argument model was substituted.

## Singleton constructor findings

Original singleton constructor 0x29798 calls ObserverBase C2 at owner+4,
constructs five locks (+0x24/+0x48/+0x6c/+0x90/+0xb4), initializes four
pointer arrays and allocators, and constructs AsyncObject at +0xe8. Array
flag initialization preserves existing upper three bits and sets low bits
0x11. It queries external-screen UI style/size, initializes state/label data,
and allocates a 0x90-byte timer object including CGUITimerInstance plus its
own member closure. Therefore the previously recovered UI list operations
cannot justify allocating a zeroed singleton or replacing its timer with a
no-op. Full singleton/timer construction and destruction remain pending.

Update 2026-10-26: original/rebuilt shared-control behavior is now tested
with real NDK libc++, and both broadcast channels are attached to Cruise.
See CRUISE_BROADCAST_WIRING.md for coverage and remaining construction limits.
