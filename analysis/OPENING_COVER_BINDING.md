# Opening Fragment Cover binding (2026-10-21)

Original V21 SHA256: 6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3.

Original AutoOpeningFragment::setCoverUtil entry 0x3bdbc receives Fragment in r0 and CoverUtil in r1. Packet onInitEnd tail at 0x2f1ba adds 0xc0 to Packet then tail-calls this setter at 0x2f1ca. Opening initializes its +0x40c pointer to zero at 0x2edf2. Another caller at 0x35728 passes null. The previously verified destroy-view call at 0x35bd0 reads the same +0x40c pointer. These observations establish non-owning storage of Packet's CoverUtil subobject, not a separately allocated cover.

Recovered sequence:
1. Store supplied cover at Fragment+0x40c, even when null; null returns without touching services.
2. GetMapServiceHandle; null returns. Load display facility at Fragment+0x388; null returns.
3. Facility vtable byte offset 0xa8 (slot 42) yields a 32-bit device identifier; service slot 8 (byte 0x20) resolves the device. Formal names are not asserted.
4. Reload/snapshot cover from +0x40c BEFORE host CGFragment::getContext. Reload facility AFTER that call, then call slot 9 (byte 0x24) to obtain the map view.
5. Copy SurfaceConfig at Fragment+0x3a0 (80 bytes with nontrivial CGString member); pass it by value to CoverUtil::setConfig(context,device,view,config). The original passes the copy indirectly at [sp]. No extra device-null guard occurs in this caller: setConfig has its own input guards.

OpeningCoverBinding.cpp implements this sequence as an explicit stage adapter. Its input must be a genuinely constructed V21 ARM32 Opening Fragment. It does not provide object construction, missing base layouts, full onCreate/onDestroy methods, or factory registration. Do NOT call it with the empty legacy AutoOpeningFragment declaration. Fixed offsets describe the original ARM32 object; native tests use a synthetic byte projection with mocked host interfaces, not a native production object. Logging is omitted.

Validation: native ASan/UBSan tests cover null cover/service/facility, device identifier forwarding, null device forwarding, exactly one SurfaceConfig copy, and mutation of facility/cover during getContext. Five original Thumb traces independently verify these guards, dispatch order, register/stack arguments and field reload timing (host services and SurfaceConfig copy routine intercepted). Existing predicate/paint comparisons also pass as prerequisites. ARM Linux musl compile-only 39/39 passes; Android strict linking and device validation remain outstanding. No usable replacement SO has been produced.
