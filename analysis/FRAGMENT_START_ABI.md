# Fragment startup ABI — V21 ARM32

Original libInstrumentPannelLarge.so caller at 0x327d0, especially 0x32970–0x329cc, establishes:

- tryStartExistFragment(const CGIntent&, int, void*) is an instance method. r0 is the parent; r1 intent, r2 mode 2, r3 type identity.
- startNewFragment(CGFragment*, void*, int, const CGIntent&, int) is called on the parent: r0 parent, r1 child, r2 type identity, r3 mode; intent and request code -1 are stack arguments.
- The identity is a function address (0x1ac7d), not a context object. The original constructs a derived child of size 0xc48, rather than allocating the placeholder CGFragment.

See v21_verified/fragment_start_327d0.txt for original instruction evidence. Signature probe references match the host mangled symbols. The native sanitizer contract test verifies forwarding to distinct parent/child receivers; it does not establish host object layout or actual navigation behavior.

CGPackage.h now uses the canonical CGFragment declaration. Old incomplete Batch20 and LargeScreenImpl flows explicitly call undefined *Unrecovered methods; they must not resolve to real host functions with incorrect receivers/arguments. Restoring their actual parent, child factory and type identity remains required. No forwarding shim or no-op implementation is provided.

All 31 production translation units compile with Zig 0.17.0 targeting arm-linux-musleabi. This is not an Android NDK link, and no replacement shared library was produced.
