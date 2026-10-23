# V21 Android ARM32 strict link with NDK r26c (2026-10-23)

The official Android NDK r26c archive used here was
`android-ndk-r26c-linux.zip`, SHA256
`6d6e659834d28bb24ba7ae66148ad05115ebbad7dabed1af9b3265674774fcf6`.
Its `source.properties` reports `Pkg.Revision = 26.2.11394342`.
The archive passed `unzip -t` and was extracted outside the repository.
Build from the repository root after extraction:

```sh
python3 rebuild-v3/assemble.py --out /tmp/ipl-v21-android-r26c
sh /tmp/ipl-v21-android-r26c/host_libs/restore_libGbl.sh
python3 /tmp/ipl-v21-android-r26c/build_android.py \
  --ndk /path/to/android-ndk-r26c \
  --out /tmp/ipl-v21-build-r26c
python3 tests/packet_vtable_shape_test.py --ndk /path/to/android-ndk-r26c
python3 tools/inspect_packet_factory.py --check-v21
```

The builder compiled **48/48** source files with the
`armv7a-linux-androideabi21-clang++` driver, then strictly linked with
`--no-undefined` against the nine supplied V21 ARM32 host DSOs: **PASS**.
`compile.log` records all source units and `link.log` is empty on success.
The result was `/tmp/ipl-v21-build-r26c/libInstrumentPannelLarge.so`,
359332 bytes, SHA256
`999665f94ef824ac7ea011ee4badde5007eb009e3927c38c1f05dda24b5b0c62`.
It is ELF32 ARM EABI5 with the soft-float calling convention, has the
expected SONAME, and exports `StartupInstrumentPannelLarge`,
`ExitInstrumentPannelLarge` and `LogInit`. Its 12 direct `DT_NEEDED` entries
are the nine hosts plus Android `libm.so`, `libdl.so` and `libc.so`.
The build uses a static NDK C++ runtime and has no direct
`libc++_shared.so` dependency. The original V21 image has 23 direct
dependencies; this difference needs runtime validation.

The NDK Packet ABI model passes its 29 vtable relocation checks, four
address points, three secondary offset-to-top values, adjustor thunks and
RTTI base offsets. Clang emits the complete destructor's D2 alias at
primary slot zero; D1 and D2 are both accepted as the non-deleting entry.
The original factory and matching host table pass the independent ELF
inspection gate. These checks establish table shape, not a working factory.

A dynamic export comparison finds 49 visible original exports and 1198 in
the current build. **34 original exports are missing**: 17 typeinfo symbols
and their 17 type-name symbols, for `IBaseModuleEx`,
`IGuideInfoCardAdaptor`, `IMultiScreenInterfaces`, `STGPoint_`,
`BroadcastExtScreen::SwitchRouteMessage`, `GPI::CGPoint`,
`GPI::Closure::CallbackBase` and ten `hsl` observer/interface classes.
Adding these exports alone would not construct or register the Packet.

The figures and SHA256 above identify the earlier Packet ABI checkpoint.
After restoring `LargeScreenImpl`'s two interfaces, another r26c build
compiled 48/48 translation units and passed the same strict link. Its
dual-interface vtable/RTTI check passed, and missing original exports
dropped to **30** (15 typeinfo/type-name pairs). See
`analysis/LARGE_SCREEN_MODULE_ABI.md`. This still does not register Packet.
The subsequent removal of invalid `legacy_` Packet fields has passed native
syntax and mock-sanitizer checks, but the transient NDK checkout is gone;
an Android compile/link of that later revision has not been rerun.

At the next ALC checkpoint, the official r26c archive was downloaded again
and reverified against the SHA256 at the top of this file. After removing
the invalid fields and correcting the ALC stack-call ABI, **48/48** source
units compiled and the strict `--no-undefined` link passed. The resulting
development SO is ELF32 ARM EABI5, has the expected SONAME and the nine V21
host dependencies plus `libm.so`, `libdl.so` and `libc.so`; its SHA256 is
`0c63bfafbefd6d69dacea565ffd73a71a31a3dd8c34faf6ed1ba6dda06cffa8d`.
The built module's two interface vtables/RTTI and the separate NDK Packet
shape probe both pass. ARM object disassembly confirms `AlcExportFields`
places flag, zero, module, auxiliary, signature, line, name and optional
pointers in successive stack words before calling the logger; see
`analysis/MODULE_ALC_ABI.md`. This is still not a usable replacement.

At the next Packet/Opening construction checkpoint, the same verified NDK
r26c compiled **51/51** source files and passed strict `--no-undefined`
linking. The three extra ARM32 units are the Opening Activity gate, Opening
host operations and Opening host construction guard. The new SO SHA256 is
`d50eb8fb248b8c2453ffcdf305bfd7f666e4920cde1df12be46cebada8d92c9e`.
`tests/large_screen_abi_test.py`, the NDK Packet shape probe, build-output
contract, original ELF gate and native ASan/UBSan stage tests pass. The
Opening host C2/D2 and ObserverBase C2 references resolve against the
matching V21 host libraries. This stage is still rolled back, not registered,
and has not been tested on Android hardware.

This is a development build, **not an installable replacement**. The
earlier NDK r27d development build loaded on a device but GFrame failed to
load `com.autonavi.package.GIPLPacket`, followed by SIGABRT. The current
source still does not publish a constructed Packet or run the fifth
original module initializer that registers its factory. Its isolated
construction guard rolls back immediately and the legacy Packet fields
were subsequently removed; the current opaque prefix is still not a valid
host object. See `analysis/DEVICE_BLACK_SCREEN_CRASH.md`
and `analysis/PACKET_VIRTUAL_ABI.md`. No device test was run on the r26c
output.
