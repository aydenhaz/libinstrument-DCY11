# V21 device report: black display followed by app crash

The user reported black instrument display and then an app crash after the Android NDK r27d development build (SHA256 `5c0587af6f4edbc68d12001f459793bafcf4d07ca35e7011507eea1f7a6bd550`) was provided. No logcat or tombstone from this test has been received yet, so the exact faulting PC and linker/Java exception state are unknown.

## Confirmed static defect

- Original V21 has five `DT_INIT_ARRAY` callbacks (20 bytes), with Thumb address points `0x14cf9`, `0x14d31`, `0x14d69`, `0x14d9d`, and `0x14e41`. The final callback calls the host `ClassNode_CGPackage` constructor to register `CGIPLPacket` and factory `0x2eb09` under `com.autonavi.package.GIPLPacket` (see `analysis/PACKAGE_REGISTRATION.md`). The rebuilt NDK image has **no `DT_INIT_ARRAY`**.
- Rebuilt `src/module_init.cpp` only exposes `createPackageRegistration(factory)` and explicitly does not invoke it automatically, because its `CGIPLPacket` factory, host package base, and fragment constructors remain incomplete.
- `StartupInstrumentPannelLarge` still calls `CGPackageMgr::loadPackage("InstrumentPannelLargePackage", ...)` and returns true without testing its result, matching the original entrypoint but with the required registration absent in this rebuild. This supplies a concrete mechanism for a black screen; whether it is the observed failure requires logs.
- The partial `CGIPLPacket::onInitEnd` dereferences `legacy_.hostFragment_` and `legacy_.activity_` although the packet constructor only sets `active_`. If this partial object is ever entered as a live host packet, it is unsafe and can crash. This path cannot be blamed for the observed crash without a tombstone.

Do not register the incomplete packet factory solely to make the initializer count or linker output match. A safe live packet needs original host base construction, genuine fragment construction, observer ownership, and cleanup. Restore the matching original SO in the tested APK for normal use. Run `tools/V21_ARM32_crash_collect.cmd` from Windows with `adb` in PATH, then upload its ZIP to identify the actual fault before making a runtime fix.
