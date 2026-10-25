# V21 Fragment builder callback boundary — 2026-10-25

The original Packet post-load uses CGFragmentBuilder with by-value
Callable<CGFragment*()> and Callable<void*()>, a void(void*) cleanup pointer,
launch mode and CGContext*. The void* callback is the fragment **type**
callback (host getFragmentType), not a generic data allocator.

Matching libGFrame C2 at 0x162ba8 copies the callbacks into +4 and +0x18.
Each is 20 bytes: vptr, wrapper, inline word, shared method/control pair.
The constructor clones wrapper slot 3 when inline word is zero, then copies
shared ownership. createFragment at 0x162ca8 dispatches wrapper slot 2
(or method slot 5 on its alternate path) and preserves the returned pointer.
The cleanup function is at +0x2c, context +0x30, launch mode +0x34: total
0x38 bytes including builder vptr. D2 at 0x162c70 destroys type callback
before factory callback. These are host byte/relocation facts, not SDK guesses.

Added GPIPointerCallable.h for the two pointer-return signatures, preserving
wrapper clone/invoke and Callable six-slot layout. It implements only a
zero-inline member binding. Target lifetime remains external; copies own
wrapper/method state, not the target object. Adapter-private RTTI and method
allocation are not claimed identical to the original GPI implementation.

Added the host CGFragmentBuilder declaration with an opaque owning prefix,
virtual destructor and deleted copy operations, and CGPackage::_registerFragment.
The original host still supplies all builder method bodies. No fake builder
constructor, factory object, or Fragment allocation was added.

Validation:
- Native ASan/UBSan pointer callback test: adjusted secondary-base virtual
  target, pointer result, cloning, copy/reset, move, self-assignment,
  exception propagation and exact shared-state destruction.
- Android NDK r26c signature probe: Callable=20, builder=56 bytes;
  constructor/destructor/register symbols exactly resolve in V21 libGFrame;
  independent probe shared object passes --no-undefined linking.
- Host ELF check pins both wrapper clone sequences and field getter offsets.
- Production overlay: 51/51 Android ARM32 units compile and strict link passes.

Not wired into onPostLoadRes: the actual cruise/guide/preview create/type/
cleanup methods and their complete Fragment construction/lifetimes remain
unrecovered. Host-side execution of these callback objects on Android has
not been tested. Packet factory publication and registration remain disabled.
This boundary work does not fix the device black display/SIGABRT yet.
