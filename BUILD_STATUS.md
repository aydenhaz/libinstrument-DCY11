# V21 ARM32 rebuild status — 2026-10-20

NOT a usable replacement. No successful strict shared-library link.

Latest source basis: InstrumentPannelLarge_full (3).tar.gz, main f50fdd4.
Reference binary: 7.5-V21-v1-v2-v3_sign_sign.apk, ARM32 instrument SHA-256
6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3.
The reference was extracted locally and read; the earlier download blockage no longer applies.

## Reproduced checks

- All 31 translation units compile with Android NDK r27c, ARM32 API 21.
- Last persisted strict --no-undefined link failed on 25 symbols; analysis/REBUILD_LINK_ERRORS.txt records that pre-withdrawal run. The prior transient 22-symbol run was lost during workspace cleanup; the two lifecycle changes have since been recovered and host-tested. A fresh Android build is required; no successful link is claimed.
- Four duplicate definitions removed, retaining their Batch17 definitions.
- Entry contract mock test passes: initial load, duplicate-start rejection, unload, ALC stack words.
- Dice String16 tests pass with AddressSanitizer and UndefinedBehaviorSanitizer (LeakSanitizer disabled because its /proc process inspection fails in this environment). These are host tests, not Android runtime tests.

## Corrections backed by original machine code

- Startup returns zero without loading again if IsCreatedLargeScreen is already true.
- Entry ALC call uses seven stack words, not a block pointer.
- CGString 24 bytes, CGIntent 20, CGColor 8, SurfaceConfig 0x50 including its constructed CGString at +0x24.
- dice::String16 is one owning pointer to {uint32 length,uint32 capacity,UTF16 data,NUL}; its former empty constructor/null c_str stub is replaced, including ownership/copy/UTF8 conversion.
- hsl::Bundle is 8 bytes (vptr plus turbo::Bundle pointer); real host destructor is declared.
- hmi::UITaskThreadProxy is 0xf0 bytes. Thread registration uses device 2, executor ID, render-thread ID, true, and the task interface pointer at offset zero.
- CGFrameLayout allocation is 0x464 bytes. LayoutParams is a nontrivial 0x58-byte sret object with host destructor; width/height are at +0x10/+0x14. Root background uses CGDrawablePtr and incStrong/decStrong on its RefBase subobject at +4.
- Verified named enum, const and namespace corrections; UTF16 format literals in earlier fixes.

## Remaining functional and ABI blockers

- Original .init_array has FIVE entries: 0x14cf9,0x14d31,0x14d69,0x14d9d,0x14e41. module_init.cpp is still a nonfunctional four-entry skeleton; factory registration has not been recovered. Adding a fifth empty constructor would not fix this.
- CORRECTION: +0x36c (map surface), +0x37c (network), +0x430 (instrument) belong to the newly allocated 0x440-byte Opening Fragment (r6), NOT CGIPLPacket. Packet factory allocates 0x16c bytes, initializes script binding at +0xc0 and proxy pointer at +0x168. Current classes still do not reproduce these layouts.
- The primary Packet vtable has 16 slots. Imported getExecutorId/getUIThreadId entries have zero raw words plus R_ARM_ABS32 relocations; the previous scanner stopped there incorrectly. See analysis/v21_verified/packet_vtable.txt.
- Four wrongly owned Batch7 methods are withdrawn, with source retained in analysis/rejected. This is removal of invalid implementations, not restoration of their real behaviors.
- Closure callbacks, ownership and missing internal business methods remain incomplete. No empty callbacks, unresolved-symbol allowances or duplicate-symbol allowances were added to make a misleading SO.
- Many expanded functions are commented call/index records. Some classes are separately re-declared with inconsistent definitions across translation units. Compilation does not establish C++ ODR or behavior equivalence.
- This target is ARM32 for V21, not an arm64 drop-in for 9.5.
- No Android process/device runtime validation has been performed.

## Build

```sh
sh host_libs/restore_libGbl.sh
python3 build_android.py --ndk /path/to/android-ndk-r27c
```

The build deliberately removes failed final outputs. It will not label an incompletely linked file as a replacement.

## Recovery verification

- Reference APK and source archive recovered after environment cleanup; reference SHA-256 rechecked.
- refreshGFramePaint restores sendMsg(0x30,0,0,source-token,nullptr), including line 0xe0 in token high word.
- onPostAlreadyLoadRes dispatches primary vtable slot 14. Both use the verified seven-stack-word Packet ALC call.
- tests/packet_lifecycle_contract_test.cpp passes with host mocks. This validates call arguments and dispatch only, not Android loading or complete class layout.

## Further verified corrections

- The first two init entries are guarded lock wrappers, calling asl::LockLinux at 0x14f24. They are not Fragment factory registrations. The fifth init registers CGIPLPacket; the four legacy empty constructors remain nonfunctional.
- Original onPreLoad allocates 0x10 and 0x24 bytes for member-callable and shared-control machinery, NOT 0x16 and 0x36. Current onPreLoad remains unreconstructed and must not be used as proof of callback scheduling.
- MemberCallableWrapper now models the wrapper's four vtable entries and target/member pointer representation. Host ASan/UBSan tests pass for direct invocation, clone ownership, and virtual dispatch through a secondary base. It is deliberately not wired to the unrecovered Callable/shared-control ABI. ARM size assertions await the restored NDK.
- NDK download made no progress in this environment and was stopped. No fresh Android compile/link result is claimed.
- Build output is cleared before compilation; the linker writes a pending file, published only on success. The fake-compiler contract test passes compile failure, missing hosts, link failure and success cases. This tests artifact publication, not Android compatibility.

## Void callback model checkpoint

- Recovered the separate wrapper and shared-method ownership model. GFrame's 0x129256 copy clones wrapper slot 3 and copies shared ownership at +12.
- Corrected interpretation of Callable slot 4: callback equality, not invocation.
- VoidCallable model and ASan/UBSan tests added; not integrated with GPI or Android scheduling.
- Correct scheduleTaskHelper declaration added. Host probe emits exactly the original imported mangled name. No new Android build or usable SO is claimed.
- See analysis/CALLABLE_RECOVERY.md for remaining RTTI, libc++ and object-lifetime blockers.
