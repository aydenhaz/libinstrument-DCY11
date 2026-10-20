# Image allocation and size ABI

Original CoverUtil::setConfig allocates 0x1e0 bytes at 0x1849c before invoking CGImage C1, rather than the one byte provided by the previous empty stub class. The host image constructor and create function write internal state well beyond a one-byte allocation. CGImage now has an opaque 480-byte, four-byte-aligned extent; inheritance, internal fields and ownership are not fabricated. Calls still use the external host constructor/destructor.

Original CGSize RTTI identifies STGSize_ at +4 after its vptr. Host CGImage::create reads width/height at reference+4/+8, and original caller constructs that 12-byte object on its stack. The old plain two-int struct put width at +0 and was ABI-incompatible. CGSize now has the observed polymorphic layout. create returns success in r0 (original caller tests it and host tail returns 1), so its former void declaration is corrected to bool. Original CoverUtil uses color value 0x101 with a four-byte pixel stride; the existing RGBA label now carries that observed value.

Validation: native ASan/UBSan test allocates CGImage and lets a stand-in host constructor write all 480 bytes, then verifies size, format and success/failure forwarding. ARM32 type extent checks pass. ARM probe allocates 480 bytes, constructs dimensions 640/480 at the corrected offsets, and passes color 257 to the exact host create symbol. All 32 production sources pass ARM Linux musl compile-only checking.

The allocation contract mock does not validate the real host constructor, allocator compatibility or device rendering. CGImage remains opaque; adding virtual methods, casting through invented bases or changing ownership still requires evidence.

Additional setConfig finding: the full original log signature is `void NS_MultiScreenBase::CoverUtil::setConfig(GNS_FRAME::CGContext *, bl::IMapDevice *, bl::IMapView *, NS_MultiScreenBase::SurfaceConfig)`. SurfaceConfig is passed by value, with the nontrivial argument indirectly addressed from the stack. Original stores context/device/view at +8/+0xc/+0x10, copies config into +0x20, obtains enable/threshold policy, and initializes image/buffer state. This full method is not yet implemented; the earlier free-function skeleton does not represent it. Evidence is saved for continuing that reconstruction.

No Android strict link or usable replacement SO.
