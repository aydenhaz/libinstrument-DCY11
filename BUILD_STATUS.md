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
- CORRECTION: +0x36c (map surface), +0x37c (network), +0x430 (instrument) belong to the newly allocated 0x440-byte Opening Fragment (r6), NOT CGIPLPacket. Packet factory allocates 0x16c bytes, initializes CoverUtil base at +0xc0 and proxy pointer at +0x168. Current classes still do not reproduce these layouts.
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

## Packet teardown checkpoint

- Corrected cancelScheduleTaskHelper and closeAllFragments to instance declarations; previous static declarations omitted this at call sites.
- onDestroy now clears render callbacks, hides FPS with size 24, cancels one refresh-identity task, closes fragments and removes device-2 UI thread registration. Host boundary test passes.
- Removed invented observer removal and post-load cancellation. ScriptThis was also invented from method equality and is withdrawn.
- Locking, map facility destruction, proxy deletion, singleton clearing and real Packet construction remain incomplete. These partial changes do not make onDestroy or the library replacement-ready. See analysis/PACKET_TEARDOWN_RECOVERY.md.

## Map teardown and task locking

- Restored engine-5 facility query through V21 service slot 15, device-ID query through facility slot 42, device-ID ALC logging and DestroyDisplayFacility before UI-thread removal.
- Restored host Packet+8 lock/unlock around refresh task cancellation. The actual host base must have been constructed; the current empty constructor remains a blocker.
- Shutdown contract test passes service-null, facility-null, facility-present and logging-disabled paths; it verifies cancellation occurs under the lock and fragment closure occurs after unlock.
- Corrected onDestroy trace name to the original "CGIPLPacket destructor". Proxy deletion at +0x168 and singleton clearing are still missing. No Android build or replacement SO is claimed.

## Proxy ownership and ARM layout checkpoint

- Task proxy declares its real virtual destructor; storage includes the vptr within 0xf0 bytes.
- Packet proxy field moved to verified +0x168; unused invented fields removed. Host/script prefix is explicitly unconstructed and legacy initialization still needs replacement.
- onDestroy now deletes the proxy after thread removal, resets its pointer and clears the rebuilt singleton. Host ASan/UBSan tests check virtual destruction, singleton timing and repeated cleanup.
- ARM EABI compile-only checks pass (Zig 0.17.0, arm-linux-musleabi): Packet 0x16c, proxy offset 0x168, task proxy 0xf0, Callable 20, method object 24. This is not Android ABI/runtime validation.
- Alternate Android-target attempt compiled 14/31 units; missing Android C headers prevented the remaining compilation. No new strict-link result or replacement SO. build_android.py now supports an explicit --cxx launcher and records its path; --ndk remains supported.

## RTTI correction and SurfaceConfig defaults

- Direct ELF RTTI parsing proves Packet inherits CGFragmentPackage at +0 and CoverUtil at +0xc0. CoverUtil inherits MapSurfaceViewObserver at +0 and IBLMapBusinessDataObserver at +4. Prior references to a script subobject were incorrect and have been corrected.
- SurfaceConfig constructor restored nonzero defaults: +0x1c=-1, +0x20=8, +0x48=1. Former all-zero initialization was incomplete. Host ASan/UBSan tests check defaults and CGString construction/destruction; ARM compile-only checks pass.
- CoverUtil and host construction still require actual vtables and callback implementations. No factory is enabled and no replacement SO is claimed.

## Typed map observer interface

- Recovered six-argument MapSurfaceViewStatusChange interface, callback slot 0 and destructor slots 1/2. Host ASan/UBSan dispatch/deletion test and ARM extent/vtable checks pass.
- Typed Add/RemoveMapSurfaceViewObserver probes match actual host symbol names. The incomplete standalone removal helper now uses the correct pointer type.
- Incorrect Packet registration still uses an explicitly unresolved legacy overload; it needs the real Opening Fragment observer. No unsafe conversion hides the blocker.
- RTTI tool now resolves defined-symbol ABS32/GLOB_DAT pointers as well as imported types. Original Packet/CoverUtil output is unchanged; a libGbl regression probe passes.
- CoverUtil implementation, proper Packet construction, strict Android linking and device validation remain incomplete. No replacement SO.

## Complete ARM32 compile-only check

- Current source passes all 31 translation units with Zig 0.17.0 targeting arm-linux-musleabi. This is compile-only, not Android/NDK linking; see analysis/ARM32_COMPILE_ONLY.md and its log.
- Removed Batch14's incompatible duplicate Closure class uncovered by the full check; callbacks remain incomplete.
- SetNaviType now uses the actual global ENaviMode_/ECruiseMode_ types. Integer forwarding test passes and ARM probe matches the original exported host symbol.
- Added --compile-only builder mode, with tests confirming it removes stale shared outputs and never links/publishes an SO.
- Full Android strict link, object construction and behavioral validation remain outstanding. No replacement SO.

## External-screen payload and hidden-return ABI

- Replaced invented namespace-local packed 48-byte payload with global tagSTExtScreen (16 bytes) and separate tagSTExtScreenControl (36 bytes).
- GetStartOpenScreenMode returns a 16-byte structure through hidden r0 storage, with this/key in r1/r2. Old int return declaration shifted arguments incorrectly. Setter takes the payload by value, not const reference.
- Protocol pointer/reference types now match actual host symbols. Control payload uses the observed 0 + eight 99 sentinel initialization; state population still incomplete.
- Host ASan/UBSan contract test and ARM calling-convention probes pass. Four probe symbols match original host definitions. All 31 units still pass ARM compile-only checks.
- See analysis/EXTERNAL_SCREEN_ABI.md. No full Android link, runtime equivalence or replacement SO.

## Fragment startup receiver and parameter ABI

- Recovered instance lookup and parent.startNewFragment(child, identity, mode, intent, requestCode) signatures from original ARM32 instructions. Removed duplicate CGFragment declaration in CGPackage.h.
- Old incomplete callers now reference explicitly undefined Unrecovered methods; their invented parent/child allocation paths cannot silently bind to real host functions. Correct derived factories and receiver recovery remain outstanding.
- Native ASan/UBSan argument/receiver contract test and ARM symbol probe pass. All 31 source units pass ARM Linux musl compile-only checking. See analysis/FRAGMENT_START_ABI.md.
- No Android strict-link success, working navigation or replacement SO is claimed.

## Assertion ABI recovery

- I_Assert now takes a Boolean condition; original calls pass a normalized Fragment state, not a string address. Removed two fabricated context-string calls from incomplete flows rather than substituting an unverified condition.
- Original release host implementation is BX LR; no rebuilt no-op implementation was introduced. Original Fragment state and receiver recovery remain necessary. See analysis/ASSERT_ABI.md.
- ARM signature probe confirms literal true/false use r0=1/0 and import I_Assert. All 31 production units still pass ARM Linux musl compile-only checking; no replacement SO.
