# Fragment receiver symbol repairs

The linker previously reported unresolved synthetic names such as
`CGFragment::isTopFragmentUnrecovered()` and
`CGFragment::inflaterUnrecovered(...)`. Those names do not exist in the supplied
`libGFrame.so`; the host exports the instance methods `isTopFragment()`,
`inflater(int, CGViewGroup*)`, and `findViewById(int)`.

The original function database records calls from fragment lifecycle functions
to these methods, and the V21 vtable inventory lists the relevant
`NS_MultiScreenBase` Fragment classes. The rebuilt member methods now forward
their actual `this` pointer to those host methods. Two additional Batch11 free
helpers omitted `this` entirely and had no callers; those invalid wrappers were
removed instead of retaining imports that could only pass a fabricated object.

The Packet render callback had a different issue: its implementation was
missing while the rebuilt CoverUtil already had a private canvas dispatcher.
Packet now forwards into that dispatcher. This removes the unresolved symbol
but does not reconstruct Packet/Opening Fragment creation or callback
registration order.

Latest validation (Zig 0.17, ARM Linux musl probe): 38/38 translation units
compile. Strict link reports 11 undefined symbol categories; see
`STRICT_LINK_ARM32_MUSL.log`. The remaining unresolved calls are not suppressed
or implemented as empty functions. This is not an Android NDK link and does
not produce a replacement `.so`.
