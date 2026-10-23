# V21 ARM32 device report: black display and SIGABRT (2026-10-23)

The tested Android NDK r27d development build has SHA256
`5c0587af6f4edbc68d12001f459793bafcf4d07ca35e7011507eea1f7a6bd550`.
The user supplied `V21_ARM32_CRASH_19410.zip`; its logcat confirms:

| Time | PID/TID | Observation |
| --- | --- | --- |
| 13:53:43.849 | 6183/6207 | InstrumentPannelLarge load begins from the app's `lib/arm` directory |
| 13:53:43.859 | 6183/6207 | Native library load returns `ret=1` |
| 13:53:43.859 | 6183/6207 | GFrame `loadPackage` line 175: `Failed to load package InstrumentPannelLargePackage with type com.autonavi.package.GIPLPacket.` |
| 13:53:43.859 | 6183/6207 | `amap_so` reports signal 6, SIGABRT, on the same thread |
| 13:53:49.416 | 6183/6207 | `amap_so` begins re-raising signal 6 after crash handling |
| 13:53:49.467 | 6183 | Zygote reports process exited due to signal 6 |

The SO loads successfully, so this failure is not an unresolved linker symbol. The
missing package is the first concrete error, immediately followed by SIGABRT
on the same thread. The collector found neither a new Java crash entry nor a
usable tombstone for PID 6183. The faulting PC and exact abort call site remain
unknown. Other errors elsewhere in logcat do not establish the cause of this
instrument startup failure. The complete log is not committed because it
contains unrelated device and application data.

## Verified missing registration

The original V21 has five `DT_INIT_ARRAY` callbacks, with Thumb address points
`0x14cf9`, `0x14d31`, `0x14d69`, `0x14d9d`, and `0x14e41`. The final
callback calls host `ClassNode_CGPackage` to register `CGIPLPacket` and
factory `0x2eb09` under `com.autonavi.package.GIPLPacket`. The tested
rebuild has no `DT_INIT_ARRAY`. `src/module_init.cpp` exposes registration
but does not invoke it, because a valid Packet factory is missing.

`StartupInstrumentPannelLarge` requests that package from `CGPackageMgr`
and preserves the original return behavior. The missing registration is
consistent with the observed GFrame error. Without a tombstone or instrumented
host trace, this does not establish which component initiated SIGABRT.

## Why the factory cannot just be registered

Original factory `0x2eb08` allocates `0x16c`, calls imported
`CGFragmentPackage(CGContext*)` on the allocation, constructs `CoverUtil`
at `+0xc0`, clears the proxy at `+0x168`, installs four Packet vtable
address points at `+0`, `+0x40`, `+0xc0`, `+0xc4`, and finally
publishes the singleton. See
`analysis/v21_verified/packet_factory_runtime_table.md` and
`tools/inspect_packet_factory.py`.

The rebuilt Packet constructor no longer publishes its pointer. Its byte array does
not construct the host package, its lock, the CoverUtil bases, or the fragment
lifecycle. Its partial `onInitEnd` reads uninitialized legacy fragment and
activity fields. Registering a factory for this object would permit GFrame to
dispatch through invalid base and virtual state.

Do not ship this development build as a replacement. A usable rebuild still
needs real host and CoverUtil construction, callbacks, fragment ownership,
and teardown, followed by device validation.
