# V21 ARM32 Android NDK build (2026-10-23)

The official Linux NDK r27d archive is `https://dl.google.com/android/repository/android-ndk-r27d-linux.zip` (SHA1 `22105e410cf29afcf163760cc95522b9fb981121`, `Pkg.Revision = 27.3.13750724`). It was downloaded and verified before extraction. This environment's `unzip` left five archive symlinks as small regular files; the `clang++` entry had to be restored as a symlink to `clang` before compilation.

From the repository root, with the NDK installed outside the repository:

```sh
python3 rebuild-v3/assemble.py --out /absolute/path/to/v21-work
python3 /absolute/path/to/v21-work/build_android.py \
  --ndk /absolute/path/to/android-ndk-r27d \
  --out /absolute/path/to/android-arm32-r27d
```

The assembled build compiled **46/46** translation units and linked with `--no-undefined`: **PASS**. The NDK launcher targeted `armv7a-linux-androideabi21`; output is an ELF32 ARM EABI5 shared object, ARMv7/Thumb-2, soft-float calling convention, SONAME `libInstrumentPannelLarge.so`. The three named public entrypoints `StartupInstrumentPannelLarge`, `ExitInstrumentPannelLarge`, and `LogInit` are present. The produced file is 355400 bytes, SHA256 `5c0587af6f4edbc68d12001f459793bafcf4d07ca35e7011507eea1f7a6bd550`.

`build_android.py` now links the NDK libc++ statically by default (`--stl static`), matching the original image's lack of a direct `libc++_shared.so` dependency. `--stl shared` remains available for a separate comparison build; it also passed a strict link but adds a runtime `libc++_shared.so` dependency. Both variants use the nine supplied ARM32 host DSOs. The static variant's direct `DT_NEEDED` list contains those nine plus `libm`, `libdl`, and `libc`; the original has 23 direct dependencies.

This is a **linkable development build, not a verified replacement**. Dynamic-symbol comparison finds 34 original exports absent from the rebuilt library, mainly RTTI/type-name entries for unrecovered interfaces, and a far larger set of new exports. The original's package/Fragment construction, AutoExscreen observer registration and lifecycle, and other host interactions are still incomplete. The device test confirmed that the SO loads, but GFrame fails to load `GIPLPacket` and the app exits by SIGABRT. See `analysis/DEVICE_BLACK_SCREEN_CRASH.md`. A successful strict link only checks symbols needed by these compiled objects against the provided libraries; it does not establish complete V21 ABI or behavior.
