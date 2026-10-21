# A106 construction/destruction correction (2026-10-21)

V21 original instrument SHA256 6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3.

The primary table at 0x6bce8 belongs to CGInstrumentPannelLargeGuideGuangQiA106Fragment (RTTI 0x6c040), whose direct base is CGInstrumentPannelLargeGuideFragment (RTTI 0x6e6d0). Primary address point 0x6bcf0 has 88 slots before the secondary header at 0x6be50 (offset-to-top -0x14). It is not a four-slot base hierarchy.

Slots 0 and 1 point to 0x31474 complete destructor and 0x31528 deleting destructor. The signature string at 0x54fc7 explicitly contains ~CGInstrumentPannelLargeGuideGuangQiA106Fragment(). Complete destructor restores 14 vptrs, uses a stack FT logging object, then invokes Guide base destructor 0x4292c. Deleting destructor calls that complete destructor then operator delete through veneer 0x45574. Slots 2/3 at 0x156e0/0x156e4 load raw bytes at this+4/this+5; they neither return constant null nor adjust this. Do not infer semantic flag names or bool normalization from these instructions.

The actual construction is inlined at 0x2e928: allocate 0x8a0 (2208) bytes, call Guide constructor 0x4281c with launch mode 1, install 14 derived vptrs, and construct/destruct a stack FT object. Context is set afterward at 0x2e9a8. The log signature is at 0x54ec7. Helper 0x165f0 receives the stack object (not Fragment), clears 0xb4 bytes and copies two trace strings with limits 0x1d/0x95 into offsets 0/+0x1e. Its companion 0x16674 emits the End trace. Earlier BaseCtor descriptions were wrong.

Retired unused Fragments_aligned.h declarations and preserved them under rejected/. They placed an extra virtual destroy() after a virtual destructor (which already supplies complete/deleting slots) and invented a four-slot hierarchy. No source currently includes this header. No new factory is enabled: the Guide base constructor, real inheritance and lifecycle still require recovery.

Original instruction tests verify all 88-slot boundary markers, raw status getters including 127/255, exact 14 vptr stores with other bytes preserved under mocked callees, complete/deleting destructor order, and actual inlined allocation/base-construction sequence. Host allocation, base ctor/dtor and FT calls are intercepted, so these tests do not prove a valid reconstructed Fragment exists. This checkpoint retires an unused header and adds evidence/tests; previous 39/39 compile-only baseline retained, no new Android link or replacement SO.

Guide base construction at 0x4281c is not a simple vptr installer: it first calls AutoNaviFragment constructor 0x23b7c, then allocates and initializes an owned 0x40-byte trace/operation object at +0x87c, in addition to installing base state. The A106 inlined sequence must not be translated into only a fixed byte array/vptr writes and treated as a live object. Base layout, nested ownership and teardown remain necessary.
