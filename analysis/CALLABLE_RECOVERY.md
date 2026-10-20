# V21 void callback recovery checkpoint

Not wired into GFrame; not a replacement library.

Original Packet onPreLoad (0x2f464) binds Packet::refreshGFramePaint (0x30d35),
constructs the callback, and calls scheduleTaskHelper with delay 1000, integer
arguments 0/-1, refresh function address as cancellation identity, and a source token
whose high word is line 0x110. The task holds a copy; the caller then destroys
its local callback at 0x2f59e. The target Packet must outlive scheduled work.

## Confirmed representation

Callable occupies 20 bytes on ARM32:

| Offset | Field |
| --- | --- |
| 0 | vptr |
| 4 | owned wrapper pointer |
| 8 | inline-dispatch word (zero in this binding) |
| 12 | shared method object pointer |
| 16 | shared ownership control pointer |

Callable virtual slots: complete destructor, deleting destructor, empty,
reset, callback equality, method-object getter. Slot 4 compares the method
objects; 0x16414 was previously misdescribed as generic invocation.

The wrapper has destructor/destructor/invoke/clone slots. GFrame copy code
0x129256 calls wrapper slot 3 to clone it when the inline word is zero, then
copies shared ownership at Callable+12. This confirms separate cloned-wrapper
and shared-method lifetimes. V21 destructor releases the shared object before
the wrapper; reset clears the wrapper before releasing shared ownership.

The method object has slots: equality, complete/deleting destructor,
target-field address, member-pointer-field address, invoke. The original
24-byte method object contains target, ARM member pointer, and an empty shared
ClassFuncInvoker pointer. Its shared allocation is 36 bytes in this binding.
RTTI names identify GPI::internal::GClassMethod and libc++ __shared_ptr_emplace.

## Implemented and checked

internal/abi/VoidCallable.h is an isolated model using the recovered field and
virtual-slot order. Host ASan/UBSan tests cover equality, deep wrapper copy,
shared method lifetime through copy/reset, move, self assignment, repeated reset,
and virtual invocation through a multiple-inheritance secondary base.

The real scheduleTaskHelper declaration now uses nested Closure::Callable,
three integers, a void pointer and an unsigned long long. A host compile plus
nm yields exactly the original imported mangled name:

    _ZN9GNS_FRAME13CGAsyncObject18scheduleTaskHelperERN3GPI7Closure8CallableIFvvEEEiiiPvy

This verifies declaration spelling, not ARM register placement or execution.

## Unresolved before integration

The model has different C++ RTTI identities from real GPI types. Native host
libstdc++ tests do not establish Android libc++ control-block ABI. ARM size
assertions have not run since NDK recovery failed. Inline bindings and other
callback signatures are not implemented. Existing fake Closure::bind calls
are unchanged and still unresolved. Packet/base-object construction and task
cancellation lifetime are incomplete. Do not reinterpret-cast this model into
a real GPI Callable or register it as a working callback.
