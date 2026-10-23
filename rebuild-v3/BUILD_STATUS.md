# V21 ARM32 rebuild status — 2026-10-23

NOT a usable Android replacement. The ARM32 Linux/musl ABI probe strictly
links, but the Android NDK and a device runtime validation are absent. Several
unrelated fragment and observer bodies in the wider rebuild remain provisional.

## 2026-10-23 long control callback and strict-link checkpoint

- Recovered the long V21 0x2ace0 callback's engine/mode gate, singleton scene
  snapshot, requested mode and level updates (including -2/-3 zoom guards),
  CrossControl, EagleEye and preview collection dispatch, map binding reset,
  three map-style selectors, and 36-byte HMI status reply. The reply preserves
  input words 7 and 8; 0x2be04 replaces words 0 through 6 only after a second
  successful map-view lookup. Diagnostic logging is omitted. The original
  three owning scene strings are copied, not byte-copied.
- Isolated the original MapView virtual slots and host MapStyleParam
  initialization in `AutoExscreenMapStyle.cpp`. The status path is isolated in
  `AutoExscreenControlStatus.cpp`. Native ASan/UBSan mock-vtable tests cover
  style selectors, short/empty names, attached/night fallback, status fields,
  provisional zoom/mode and the failed second lookup. No Android runtime test
  of the observer's ownership or host objects has been performed.
- Reassembled and compiled 46/46 ARMv7 Thumb translation units; strict link
  against the nine supplied ARM32 hosts **passes**. This output targets
  Linux/musl and cannot be installed as the V21 Android library. The original
  ELF has 23 `DT_NEEDED` entries, whereas the probe has nine host entries plus
  `libc.so`; the dependency/runtime mismatch remains material. No Android NDK
  is present in this environment, and a Zig Android target cannot compile the
  libc++ headers without an Android sysroot.
- The original observer is allocated lazily in three 0x128-byte screen
  instances at 0x296a0 and constructed at 0x29798. The current rebuild has no
  equivalent constructor, registration, or lifetime path; see
  `analysis/v21_verified/auto_exscreen_observer_lifecycle.md`. Strict linking
  does not make either restored callback reachable in a live host process.

## 2026-10-23 map status operational path

- Recovered V21 0x2c070's operational order in an isolated dispatcher that
  accepts a constructed per-screen config. It snapshots the only two scene
  values actually consumed (+0x88/+0x8c), preserving their values before
  UI or map changes while avoiding a raw copy of three owning strings.
  The map-view entry gate, UI writes, dynamic-level close, instance mode and
  level calls, and binding reset are covered by a mock host test.
- Restored the timer gate at 0x2c1a4..2c1c8, calling the imported
  `CGUITimer::start(int,unsigned long long)` on timer+0x10 with interval +4
  and 64-bit value +8. Host ASan/UBSan tests cover its guards, failed and
  successful starts, and one-shot started byte.
- The original observer callback and per-screen config singleton still
  require Android runtime validation. The short observer callback now calls
  the dispatcher after its original map-view gate, and three 0x340-byte
  singletons are initialized lazily with the imported scene configuration,
  `asl::Mutex`/`LockLinux`, and ARM32 12-byte strings. The long control
  callback remains unrecovered.
- Latest ARMv7 Thumb Linux/musl ABI probe compiles 43/43 translation units.
  Strict link now fails only on
  `OnSetExScreenMapStatusUi(tagSTExtScreenControl const&)`. This probe does
  not produce a usable Android library or validate live singleton ownership.

## 2026-10-23 map view mode dependency

- Restored V21 0x1def4's instance `AutoMultiScreenConfig::SetMapViewMode`:
  both view lookups, the +0x314 state update, modes 0/1/2 mapped to 2/1/0,
  mode 3 resolved through the engine-1 reference view, and 99 fallback.
  Mock-vtable ASan/UBSan tests cover modes, null views and invalid values.
- Restored 0x1781c's scene 0/1 dynamic-level close step. Host mock-vtable
  tests verify `IsOpenDynamicLevel` and conditional `OpenDynamicLevel` on
  separate ARM32 four-byte virtual slots. The 0x1e084 `SetMapLevel`
  jump table and its +0x338 stored-type cleanup are documented in
  `analysis/v21_verified/map_view_virtuals.md`. The isolated 0x1765c
  dynamic initializer now writes the 0x88-byte parameter object, then
  dispatches `InitDynamicLevel`, `OpenDynamicLevel(true, type)` and
  `OpenDynamicCenter(false)`; mock-vtable ASan/UBSan tests check the record
  and order. The 0x1e084 instance `SetMapLevel` now implements its jump
  table, 99 fallback/clamp and +0x338 state transitions using the verified
  four-byte ARM virtual slots. Host mock-vtable tests cover those paths.
  The static initializer's singleton-owning wrapper remains absent.
- Reassembled this source and compiled 40/40 ARMv7 Thumb translation units
  with the Linux/musl ABI probe. Its strict link still fails on exactly
  `OnSetExternalScreenMapStatusUi(int,int,int,int)` and
  `OnSetExScreenMapStatusUi(tagSTExtScreenControl const&)`. Both instance
  methods have the expected ARM32 C++ symbols. This is not an Android build.
- Removed Batch13's incorrect empty `GetMapViewModeConfig` pseudo-method,
  which had mislabeled the status writer at 0x2be04.

## 2026-10-23 map status callback branch analysis

- Resolved V21 `libhsl.so` MapViewBaseImpl vtable relocations against the
  original callback sites. `word[8]` drives the map-style methods
  `SetSpecifiedMapStyle`, `SetMapStyleName` and `SetMapStyleTime`; the
  original +0x3e8 call is `OpenDynamicLevel(false, type 0)`, not a reset.
  Corrected the plan field name accordingly. The HMI status tail copies
  the 36-byte input record before rewriting only words 0–6, so words 7–8
  are preserved. See `analysis/v21_verified/map_view_virtuals.md`.
- Removed the lingering static `SetMapLevel(int,int)` pseudo-body from the
  Batch12 overlay. Its C++ symbol matched the instance method while using
  the wrong ARM register arguments; keeping it would misroute a restored
  callback at runtime.
- Verified 0x2ace0's engine/mode entry gate, EagleEye all-mode fanout,
  level-step guards, three view-state selectors, observer collection
  dispatch and HMI status tail. The original `ControlAllMultiModeFlag`
  tests bit 0 and `GetMultiViewModeInfo` returns bool; the host declaration
  has been corrected and the EagleEye UI writer branch restored in an
  isolated helper with ASan/UBSan host tests.
- V21 0x2c070's fourth integer affects its log only. Its temporary scene
  record uses three 12-byte copy constructors and matching destructors,
  so raw copying the full 0xe4 bytes would corrupt ownership. Both callbacks
  remain incomplete; see `analysis/v21_verified/auto_exscreen_targets.md`.

## 2026-10-23 top-render callback dependency checkpoint

- The original V21 library imports `CGWorkStation::setTopRenderCallback` with a
  plain canvas function pointer, has no `libc++_shared.so` in `DT_NEEDED`, and
  does not import or define the `bad_function_call` destructor. The rebuilt
  CoverUtil had introduced that additional symbol through private
  `std::function` storage. Replaced that storage with an atomic owner pointer
  and the existing plain callback dispatcher. Existing host paint tests now
  also cover replacement, synchronous registration and callback removal under
  ASan/UBSan.
- Reassembled and compiled 40/40 ARMv7 Thumb translation units with the
  Linux/musl ABI probe. Its strict link has **two** remaining undefined
  symbols: `OnSetExternalScreenMapStatusUi(int,int,int,int)` and
  `OnSetExScreenMapStatusUi(tagSTExtScreenControl const&)`. These are substantial
  unrecovered behaviors; neither is replaced with an empty implementation.
  See `analysis/STRICT_LINK_ARM32_CURRENT.log`. The probe is not an Android
  build, and no replacement `.so` has been produced.
- Recovered V21 0x1ddd8's map-view lookup using its 0x16788 table: screen
  Large/Small/Widget selects engine ID 5/3/7. A host ASan/UBSan test covers
  valid and invalid selectors. Restored isolated V21 0x1c210 scene-record and
  0x3bec0 binding-record initializers, including the original untouched
  bytes. The full 0x340-byte singleton constructor and its lock and scene
  registration remain incomplete. See
  `analysis/v21_verified/auto_multi_screen_binding.md`.

## 2026-10-23 map binding checkpoint

- Replaced Batch13's `ResetMapBindingList(...) { return true; }` with the
  scene selection, 0x20-byte binding-state calculation, and map-service
  virtual registration observed in V21 0x1e790. Added the per-screen
  OnlyShowCrossImage override and UI field reader dependencies. The
  field meanings not proved by disassembly remain offset-labelled.
- Host ASan/UBSan tests pass for the UI reader/writer, binding state, and
  service dispatch (success, missing target, missing service). ARMv7 Thumb
  Linux/musl probe compiles 40/40 translation units and still reports only
  the two unrecovered map-status callbacks and the probe runtime's
  `std::__ndk1::bad_function_call` destructor at strict link.
- The invalid static SetMapLevel pseudo-body was withdrawn. The
  AutoMultiScreenConfig constructor/singleton and SetMapLevel and
  SetMapViewMode instance methods remain incomplete. No Android build or
  replacement `.so` is claimed. See `analysis/v21_verified/auto_multi_screen_binding.md`.

## 2026-10-23 UI config reader checkpoint

- Recovered V21 0x16fcc's six-field reader and three current-mode/current-scene
  wrappers (0x1748c, 0x17320, 0x17348). Missing bundles, invalid indices,
  and absent hsl::Config yield the original 99 sentinel. Host ASan/UBSan
  contract tests exercise writer/reader round trips and the sentinel paths.
- This reader supplies the field values consumed by
  `AutoMultiScreenConfig::ResetMapBindingList`; the two map callbacks remain
  undefined at this checkpoint.

## 2026-10-23 MapStatus recovery checkpoint

- Recovered the original APK and the latest GitHub source branch after the
  scratch workspace was cleaned; verified the same ARM32 binary SHA-256.
- Implemented 0x1984c's three zero-initialized scene slots and the 0x173fc
  level / 0x171b8 view-mode UI-config wrappers. Preview view mode writes to
  guide, while preview level stays in preview. Host ASan/UBSan test passes for
  both mappings, preservation of other fields, and the MapStatus 99/-1 branch
  decisions. Control record word uses verified from 0x2ace0 are recorded in
  `analysis/v21_verified/auto_exscreen_control_words.md`.
- Reassembled and compiled all 39 translation units for ARMv7 Thumb with the
  Linux/musl ABI probe. Strict link still has the same three undefined symbols:
  the two unfinished map-status callbacks and `std::__ndk1::bad_function_call`
  destructor absent from this probe runtime. The pseudo SetMapLevel and
  ResetMapBindingList methods still need real implementations before routing
  the first callback. No Android `.so` was produced.

## 2026-10-22 recovery checkpoint

- Recovered the public `rebuild-v21-v3` branch and the 7.5 V21 source and
  reference binary after workspace cleanup. The original ARM32 DSO hash still
  matches `6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3`.
- Implemented the verified 20-mode/three-scene/six-field Bundle writer and
  CrossControl observer callback. A mocked Bundle/Config host test passes,
  including preservation of unrelated fields, 99 defaults, invalid-mode
  rejection, and the dynamic-level predicate. ARM32 object layout assertions
  cover the collection and owner sizes.
- Excluded the legacy Batch20 fragment pseudocode from the build: it invokes
  receiver-less Fragment methods and tries to register a fragment pointer as
  a concrete TestNumObserver. Withdrew other synthetic narrow-format and
  receiver-less calls in Batch22 and LargeScreenImpl. Those flows are still
  incomplete, not silently implemented.
- Recovered three deferred bindings from the original allocation extents and
  call arguments: (int,bool), four integers, and one 36-byte control record.
  The nested Callable is 20 bytes on ARM32, the bound objects are 24/32/52
  bytes, and the host scheduling import now matches its member receiver,
  nested Callable argument, engine, delay, source token, and hidden shared
  return. Native ASan/UBSan bind/copy/invoke tests pass.
- Linux/musl Zig ARMv7 Thumb probe: 39/39 translation units compile. Strict
  link fails on the two unfinished map-status callbacks and the
  `std::__ndk1::bad_function_call` destructor absent from this Linux
  namespace probe. The two V21 APK variants contain no
  `libc++_shared.so`; an Android NDK runtime is still needed. This probe is
  not an Android build. No replacement `.so` was produced.
- Next: recover the two remaining map-status
  callbacks, then build with an Android armeabi-v7a toolchain and verify on a
  device. See `analysis/v21_verified/auto_exscreen_targets.md`.

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

## Fragment view receiver ABI

- inflater, findViewById and isTopFragment corrected from static declarations to instance methods, based on original host field accesses and argument forwarding. Their mangled names did not expose the prior receiver omission.
- Receiver-less placeholder paths explicitly remain unresolved. The existing derived GuideGuangQiA106Fragment now forwards its actual this in Batch18, but object construction, IDs and widget behavior remain incomplete.
- ARM probe confirms this/ID/parent register forwarding; all 31 source units pass ARM Linux musl compile-only checking. See analysis/FRAGMENT_VIEW_ABI.md. No Android strict link or replacement SO.

## Current symbol inventory

- Added an ELF object/host symbol inventory with global/weak classification, export visibility filtering and ELF identity checks. It is not a linker or ABI compatibility validator.
- Fresh 31-object ARM Linux musl inventory: 177 names supplied by hosts, 2 supplied by rebuilt objects, 43 unprovided names. These include deliberate Unrecovered blockers and runtime/toolchain differences; this is not an updated Android link error count.
- std::__1 vs host std::__ndk1 explains additional C++ boundary mismatches; do not alias incompatible standard-library ABIs. See analysis/CURRENT_SYMBOL_INVENTORY.md. No replacement SO.

## Package receiver ABI

- getContext and getHostFragmentActivity corrected to instance methods. Original getContext reads package+0x5c; the prior static call omitted the required receiver.
- Original callers pass the same package to both methods. Activity return type remains unverified and cannot be treated as a package/context source.
- Receiver-less incomplete flows explicitly remain unresolved; the Packet still lacks constructed host bases. ARM signature forwarding and 31-unit Linux musl compile-only checks pass. See analysis/PACKAGE_RECEIVER_ABI.md. No replacement SO.

## Cover render-state logic

- Extracted exact first-render predicate (including signed <= boundary) and gated surface-event counter reset from original CoverUtil instructions. Constructor evidence records threshold 2 and related default state.
- Native ASan/UBSan and ARM Thumb compile-only checks pass. Unicorn differential execution against the SHA256-verified original binary passes all 424 cases with I_GetAlcPtr mocked null.
- Helpers are not yet wired into Packet: full CoverUtil observer class, second interface and construction remain incomplete. See analysis/COVER_RENDER_STATE.md. No replacement SO.

## Cover paint state transitions

- Recovered paint/capture gates and wrapping counter increment. Original reloads state after external effects; the decision helper must be re-evaluated after drawing.
- Added 576 original Thumb paint comparisons and 2 synchronous counter-mutation cases; all pass alongside prior 424 cases. Native sanitizer and ARM Thumb compile-only tests pass.
- Device/canvas methods are mocked in emulation. Helpers remain prerequisites, not an installed CoverUtil callback. No replacement SO.

## Cover capture RGB arithmetic

- Recovered event 0x17 request/ready transitions and valid RGB/RGBA average arithmetic, preserving float32 accumulation and dimension-based divisor. Helper rejects unsupported malformed buffers; this is not invented original validation.
- 30 original Thumb/VFP arithmetic/state cases pass in addition to prior 1,002 cover-state cases. Native sanitizer and ARM Thumb compile-only checks pass.
- Stops before external formatter operations; remaining string/configuration effects and actual observer integration are incomplete. See analysis/COVER_RGB_CALLBACK.md. No replacement SO.

## Cover RGB configuration writes

- Corrected hsl::StringUtils::Format to UTF16 varargs returning owning dice::String16 via hidden ARM32 result storage. Added static config binding and helper writing widgetCoverR/G/B in original order.
- Native sanitizer host-contract test passes; ARM signature probe and all 31 production compile-only units pass. Original callback full execution trace (external string/config services mocked) verifies format/write order, temporary cleanup and ignored events.
- Helpers are not installed into a constructed CoverUtil/Packet. See analysis/COVER_RGB_CONFIGURATION.md. No Android strict link or replacement SO.

## Composed capture-result helper and secondary thunk

- Joined flag updates, valid-image arithmetic and RGB config writes into applyCoverCaptureResult. Internal result enum is not the host observer return type; malformed buffers are explicitly rejected by the helper.
- Verified second observer table has destructor slots 0/1 and callback slot 2; all thunks adjust this by -4. Primary and secondary callback emulation produce identical full traces and flag/cleanup behavior.
- Native sanitizer pipeline test and complete ARM Thumb helper compilation pass. Formal bl observer callback signature remains unresolved; helpers are not yet installed in Packet. See analysis/COVER_CAPTURE_PIPELINE.md. No replacement SO.

## Concrete CoverUtil observer class

- Located the host dice-to-bl business-observer bridge, which preserves r1/r2/r3 into bl slot 2. Added the supported binary interface shape; method spelling/const qualifiers follow the forwarded dice interface inference.
- Added a concrete dual-observer CoverUtil with original constructor state, map/capture processing and first-render predicate. ARM32 size 0xa8, 14 field offsets and 44-byte vtable group/slot order pass checks.
- Native sanitizer tests instantiate the class and dispatch through both observer bases; all 32 production units pass ARM Linux musl compile-only checking.
- Still not wired into Packet: setConfig, drawing/capture service calls, logging, host bases and registration remain incomplete. See analysis/COVER_OBSERVER_CLASS.md. No replacement SO.

## Concrete CoverUtil top-render callback

- Added actual TopRenderCoverCallback implementation using verified canvas drawImgEx and screenshot-device slots. Recovered polymorphic CGRect layout and grouped screenshot fields into a typed subobject without moving ARM32 offsets.
- Preserves request-flag ordering, false capture return handling, wrapping increments and external callback state reloads. Native sanitizer dispatch/config tests and ARM32 layout probes pass; all 32 production units pass ARM Linux musl compile-only.
- Actual device/image configuration, logging, callback registration and Packet construction remain incomplete. See analysis/COVER_TOP_RENDER_IMPLEMENTATION.md. No replacement SO.

## Image allocation and CGSize ABI

- Replaced one-byte CGImage placeholder with verified opaque 0x1e0 extent. Corrected CGSize to its 12-byte ARM32 polymorphic layout, CGImage::create success return, and observed cover color value 0x101.
- Native sanitizer allocation/argument contract and ARM probe pass; all 32 production source units pass ARM Linux musl compile-only checking.
- Recovered full setConfig signature, including by-value nontrivial SurfaceConfig. Its implementation and Packet integration remain incomplete. See analysis/IMAGE_ALLOCATION_ABI.md. No replacement SO.

## Cover configuration prefix and policy

- Added explicit prepareConfiguration prefix operation with required-input checks, pointer/config storage, by-value SurfaceConfig copy, and original list/threshold policy reads. Enabled results report NeedsImageSetup rather than claiming complete initialization.
- Preserves defaults "0"/"2", substring matching, ignored GetString status and unclamped atoi result. Context field is now pointer-typed without shifting ARM32 layout.
- Native sanitizer policy/base-interface tests and ARM32 layout probe pass; all 33 production units pass ARM Linux musl compile-only checking. See analysis/COVER_CONFIG_POLICY.md.
- Image/file/buffer setup, registration, optional closure and Packet integration remain incomplete. No replacement SO.

## Cover image allocation and screenshot-source stages

- Added explicit recreateImageStorage and bindScreenshotSource stages: destroy old image, create verified-size replacement, clean up failure, then separately bind final dimensions and host data length/pointer in original order.
- Native sanitizer lifecycle contract passes, including replacement failure and subsequent recreation. All 34 production units pass ARM Linux musl compile-only checking; getter references match host exports.
- File loading, registration, complete setConfig and Packet integration remain incomplete. See analysis/COVER_IMAGE_STAGES.md. No replacement SO.

## Cover resource image load stage

- Restored HmiResPath/type-3 resolution, direct sm_cover.png append, existence branch and exact host image-load arguments. Preserves inhibit flag on failed load and prior state on missing-file branch while setting byte +0x91.
- Native sanitizer path/state tests and host symbol checks pass; all 35 production units pass ARM Linux musl compile-only checking.
- Complete initialization orchestration, observer/closure registration and Packet construction remain incomplete. See analysis/COVER_RESOURCE_IMAGE.md. No replacement SO.

## Cover initialization pipeline and map registration

- Added initializeConfiguredCover staged entrypoint composing policy, image creation/resource loading/buffer binding and typed map-observer registration. Captures image dimensions before external image calls; internal prefix now uses const-reference config to avoid a second by-value copy.
- Explicit outcomes preserve failure/disabled suppression, optional-file continuation and incomplete optional top-render registration. Map observer sees complete buffer fields before synchronous notifications.
- Native sanitizer integration test and original instruction regressions pass; all 36 production units pass ARM Linux musl compile-only checking. See analysis/COVER_INITIALIZATION_PIPELINE.md.
- Full original setConfig/optional closure installation and Packet/module integration remain incomplete. No replacement SO.

## Cover top-render registration and setConfig entry

- Installed the optional top-render callback and added the original void setConfig entrypoint over shared initialization logic. Saves target before host registration and preserves replacement/immediate-dispatch behavior.
- Private std::function storage does not cross the host ABI; only a plain canvas callback is registered. Original GPI private allocation/refcount representation is not claimed equivalent.
- Native sanitizer callback/pipeline tests and single by-value config-copy test pass; all 37 production units pass ARM Linux musl compile-only checking. See analysis/COVER_TOP_REGISTRATION.md.
- Logging, Packet host-base/module construction and Android strict linking/device validation remain incomplete. No replacement SO.

## Cover release checkpoint (2026-10-20)

Recovered original 0x18078 cleanup effects as `CoverUtil::releaseConfiguredCover`: buffer reference reset, RGB defaults 127, observer removal, optional host/private callback clearing, image deletion. Preserves original guards and fields not reset by original. Source and evidence in analysis/COVER_RELEASE.md. Native ASan/UBSan integration passes; ARM Linux musl compile-only 38/38 passes. Original caller naming/Packet integration and Android NDK strict linking remain unfinished; no replacement .so is available.

## Cover caller and original trace checkpoint (2026-10-21)

Located release caller at 0x35bd0 in AutoOpeningFragment::onDestroyView, receiver from Fragment+0x40c. No incorrect Packet destructor wiring added: Fragment layout and base remain unrecovered. Added five original Thumb cleanup trace cases and native reentrant image-removal regression; tests pass. Quarantined wrong Batch11 CoverUtil_setConfig free function. ARM Linux musl compile-only remains 38/38; no Android strict-link success or replacement SO.

## Opening Cover binding checkpoint (2026-10-21)

Recovered Packet+0xc0 -> Opening+0x40c non-owning Cover pointer chain and setter at 0x3bdbc. Added explicit bindOpeningCover adapter for genuinely constructed V21 ARM32 fragments, preserving service/facility guards, vtable slots, field reload ordering, and one by-value SurfaceConfig copy. Native ASan/UBSan and five original Thumb binding traces pass. ARM Linux musl compile-only 39/39 passes. Adapter is not registered with the incomplete Fragment/Packet factory; no Android strict-link success or replacement SO. See analysis/OPENING_COVER_BINDING.md.

## Packet host activity boundary checkpoint (2026-10-21)

Corrected simulation activity getter from static to instance, and simulation/package activity return types from CGPackage* to CGFragmentActivity*. Packet partial onInitEnd now supplies the simulation receiver and separately reads context through Packet's package base. Removed unused invalid Batch9 activity call. Host original-instruction getter/receiver chain regression and ARM signature probe pass; compile-only 39/39. Factory/base construction and full initialization remain incomplete. Details: analysis/PACKET_HOST_ACTIVITY.md. No replacement SO.

## Simulation thread receiver audit (2026-10-21)

Corrected getId/getRenderThreadExecutorId/getRenderThreadId from static to instance and supplied ownWorkStation receivers in Packet and Batch11/12. Original host-instruction regression verifies each getter and adapter dispatch, ARM signature probes pass, and compile-only remains 39/39. Null-adapter abort branches and full Packet runtime remain untested; no replacement SO.

## Entry package-manager receiver checkpoint (2026-10-21)

Corrected getPackageMgr/getExhibition to instance methods. Startup/Exit now forward their incoming simulation argument instead of discarding it. Exhibition calls supply ownWorkStation; original getter's activity refresh verified. Entry ASan/UBSan regression with distinct receivers, original host instruction checks and ARM signature probes pass; 39/39 compile-only. No replacement SO or Android strict-link success.

## Original entry behavior audit (2026-10-21)

Executed original startup/exit with mocked host effects and verified exact intent literals, load/unload flags, object receivers, destruction order, service-null and duplicate-start branches. Confirmed startup ignores loadPackage result and returns 1 after the attempt. Native entry ASan/UBSan regression expanded and passes. Corrected stale ALC documentation. No production behavior change; previous 39/39 compile-only baseline retained. No Android load success or replacement SO. See analysis/ENTRY_ORIGINAL_TRACE.md.

## Package registration component checkpoint (2026-10-21)

Recovered host ClassNode_CGPackage layout (0x20 ARM32), imported ctor/dtor and original CGIPLPacket/class alias registration parameters. Added explicit factory-taking registration component with scoped ownership. Removed four inert placeholder module constructors; originals archived. Native sanitizer and original init instruction traces pass; compile-only 39/39. Component is not auto-registered: real Packet/base construction and other module initializers are unfinished. No replacement SO.

## Unsafe Fragment factory isolation (2026-10-21)

Found concrete one-byte allocation followed by 0x80-byte memset/embedded writes in legacy A106 factory. Quarantined Fragments.cpp/Fragments_batch4.cpp implementations and prohibited default construction of their ten empty types. Prohibited empty CGFragment construction; four fallback allocations now explicitly require an unrecovered factory. ARM negative construction contract and compile-only 39/39 pass. This removes unsafe invented code, not completes host factories. Real Packet/Fragment construction, Android linking and replacement SO remain unavailable. Details: analysis/FRAGMENT_FACTORY_QUARANTINE.md.

## Real A106 construction path checkpoint (2026-10-21)

Corrected prior factory/destructor confusion: 0x31474 is complete destructor; 0x31528 deleting destructor. Primary vtable has 88 slots; slots 2/3 read bytes +4/+5. Actual inline construction at 0x2e928 allocates 0x8a0 and calls Guide base 0x4281c with mode 1 before 14 derived vptr stores. FT helpers operate on stack logging storage, not Fragment memory. Retired unused wrong four-slot header. Original instruction regressions pass with base/allocation/log callees mocked. No recovered complete factory or replacement SO; previous 39/39 compile-only baseline unchanged.

## Startup return and A106 constructor audit (2026-10-21)

Changed exported StartupInstrumentPannelLarge declaration/definition from int to original bool. Rechecked entry ASan/UBSan and original A106 instruction traces; full ARM Linux musl compile-only result recorded alongside this checkpoint. A106 base construction calls AutoNaviFragment ctor and owns an additional +0x87c object; full Fragment definition remains incomplete. No Android strict link or replacement SO.

## AutoNavi base ownership trace (2026-10-21)

Mapped Guide->AutoNavi base construction and cleanup. AutoNavi constructor initializes through +0x89c and accesses HSL services. Guide owns a 0x40-byte object at +0x87c, deleted through virtual slot 1 before AutoNavi teardown. Added full disassembly evidence. A106 emulator and relevant package/registration/observer tests pass; earlier ARM32 compile-only is 39/39. No full base implementation or replacement SO.

## Fresh ARM32 compile and strict-link attempt (2026-10-21)

Recompiled all 39 translation units with Zig 0.17.0 targeting `arm-linux-musleabi`; 39/39 compile. A strict `--no-undefined` link against the supplied ARM host libraries then failed with 24 unresolved symbols. The blockers include deliberately unresolved Fragment construction/view entrypoints, observer registrations whose concrete observer interfaces are not implemented, missing callback bodies, and C++ ABI differences (`std::__1` objects cannot satisfy host `std::__ndk1` signatures). The detailed linker output is in `analysis/STRICT_LINK_ARM32_MUSL.log`. This is a Linux/musl toolchain probe, not an Android NDK link. No unresolved-symbol suppression, fake host implementations, or replacement library was produced.

## Undefined-symbol repair checkpoint — 2026-10-21

- Rebuilt with `/tmp/ipl-arm-cxx` (Zig ARM Linux musl), not Android NDK: all 38 translation units compile; strict host link still fails.
- Removed an unused, receiver-less `CGPackage::getContextUnrecovered()` call from `LargeScreenImpl::doStartFragment`; that call was not used by the recovered function body.
- Latest strict link log: `analysis/STRICT_LINK_ARM32_MUSL.log`. It still reports 20 undefined-symbol categories, including fake fragment snippets, incorrect `SystemNotify(void*)` observer signatures, unimplemented closure callbacks, the `std::__1` versus Android `std::__ndk1` ABI mismatch, and missing valid package/fragment receivers. These are not papered over with empty stubs or linker suppression.
- No `.so` was produced. A valid Android NDK build and recovered class/observer/callback layouts are still required.

## Observer-owner correction checkpoint — 2026-10-21

- Removed four invalid SystemNotify registrations from `CGIPLPacket::onInitEnd()`: original V21 layout evidence places those observer interfaces in the 0x440-byte Opening Fragment; Packet is only 0x16c bytes. The registration code had passed Packet `this` under wrong `void*` signatures.
- Rebuilt with the ARM Linux musl probe: 38/38 translation units compile; strict link falls from 20 to 16 undefined-symbol categories (see `analysis/STRICT_LINK_ARM32_MUSL.log`).
- Still blocked on intentionally unrecovered Fragment receivers/factories, callback/closure bodies, an incomplete AutoOpeningFragment helper, and the `std::__1`/`std::__ndk1` standard-library boundary. No fake stubs or linker suppression added; no `.so` produced.

## Android ABI target confirmation — 2026-10-21

- Confirmed the requested production ABI is `armeabi-v7a` (ARMv7-A, Thumb, softfp calling convention); the original V21 binary and supplied host DSOs report ELF32 ARMv7 / EABI soft-float ABI tag.
- Added `Application.mk` for `armeabi-v7a` / Android API 21 and explicit ARMv7-A + softfp flags for NDK builds. The custom `--cxx` launcher remains responsible for its own target configuration.
- Fresh 38/38 compile-only check passed, and the output object reports ELF32 ARM, ARMv7, Thumb-2, soft-float ABI. This used Zig's Linux/musl target, not Android NDK. No Android NDK is installed in this environment, so no Android link or `.so` is claimed.

## Fragment receiver symbol repair — 2026-10-21

- Replaced receiver-less `CGFragment::*Unrecovered` calls in reconstructed AutoNavi, AutoCruise, AutoArNavi, AutoPreview, AutoOpening, and Cruise fragment methods with the actual instance methods `isTopFragment`, `inflater`, and `findViewById`. Their original call records and fragment vtable groups confirm these are constructed Fragment receivers.
- Removed two unreferenced Batch11 helper functions that had no `this` pointer but called Fragment instance methods anyway; they were not viable implementations.
- Added the Packet top-render callback bridge to the recovered private CoverUtil dispatcher. It resolves the missing Packet callback symbol without exporting std::function through the host ABI; Packet/factory wiring is still incomplete.
- Strict link now reports 11 undefined symbol categories (down from 16), with 38/38 translation units compiling. Remaining blockers are package/fragment receiver recovery, AutoExscreen closure/action implementation, TestNumObserver base adjustment, and the `std::__1` vs Android `std::__ndk1` vector ABI. See `analysis/STRICT_LINK_ARM32_MUSL.log`; no `.so` produced.

## TestNumObserver subobject offset repair — 2026-10-21

- Corrected SystemNotify declarations to the host's typed `TestNumObserver*` signatures. Original Thumb disassembly shows AutoCruise registers fragment+0x5f0; AutoPreview registers and removes fragment+0x2e4. Source now passes those exact subobject pointers, and the produced ARM objects import the exact host mangled symbols.
- Strict link falls to 9 undefined-symbol categories with all 38 translation units compiling. Remaining blockers are AutoExscreen closure/action recovery, package receiver recovery, and the NDK libc++ vector ABI. See `analysis/v21_verified/test_num_observer_offsets.txt` and latest `analysis/STRICT_LINK_ARM32_MUSL.log`. No `.so` was published.

## Closure ABI inspection — 2026-10-21

- Inspected V21 Thumb call sites at `sub_2a5a8`, `sub_2a7e0`, and `sub_2aa28`, plus the provider in `libhsl.so`. Host API takes `GPI::Closure::Callable<void()> const&`, `bl::MapEngineID`, `unsigned int`, and `unsigned long long`; reconstructed declaration instead uses `GPI::Closure const&` and `int`.
- The call sites construct shared wrapper state and clean it up around scheduling. `Closure::bind` has no body and the source's `ActionA/B/C` targets are not independently recovered. An incomplete return-type edit failed compilation and was reverted; no placeholder callable was introduced.
- Re-run after reverting: 38/38 sources compile, strict link remains at nine unresolved categories. Current evidence is in `analysis/v21_verified/auto_exscreen_closure_abi.txt`; full linker diagnostics are in `analysis/STRICT_LINK_ARM32_MUSL.log`. No usable `.so` exists yet.

## Android libc++ namespace probe — 2026-10-21

- Tested the Zig libc++ header configuration with `_LIBCPP_ABI_NAMESPACE=__ndk1`. All 38 translation units compile, and the `AutoConfigJsonData::GetMultiViewModeInfo` import now matches the Android host's `std::__ndk1::vector` symbol; the earlier `std::__1` mismatch is eliminated in this probe.
- Strict linking still fails because the Linux musl probe lacks the Android libc++ runtime definition for `std::__ndk1::bad_function_call::~bad_function_call()`, in addition to unresolved package/fragment and AutoExscreen closure/action recovery. An actual NDK r27c link remains necessary to validate runtime resolution. This is not an Android build and no `.so` is claimed.
- Probe details and the separate linker log are in `analysis/v21_verified/android_libcxx_namespace_probe.txt` and `analysis/STRICT_LINK_ARM32_NDKABI_MUSL.log`.

## LargeScreenImpl package receiver trace — 2026-10-21

- V21 dispatch call sites load the `CGPackage*` used by `getHostFragmentActivity()` and `getContext()` from a shared BSS slot at image RVA `0x6fd48`; `LargeScreenImpl::this` is not that receiver. A separate path at `0x158aa` reads the same slot.
- The original package receiver therefore exists, but its lifecycle initializer is not yet recovered. A replacement DSO cannot safely hard-code the original RVA or substitute its own `this`; the three `Unrecovered` symbols remain intentional blockers pending receiver initialization recovery. Evidence: `analysis/v21_verified/large_screen_package_slot.txt`.

## ARMv7 shared-library build attempt — 2026-10-21

- Started full build with the Android libc++ namespace probe wrapper. All 38/38 translation units compile as ELF32 ARM objects, but the objects use Zig's Linux musl target because no Android NDK is installed in the environment.
- Strict shared-library link failed; no `.so` was produced. Remaining undefined symbols are the NDK runtime's `std::__ndk1::bad_function_call` plus the unrecovered AutoExscreen `Closure::bind`, `RunAsyncDelayByEngineId`, `ActionA/B/C`, and LargeScreenImpl `CGPackage`/`CGFragment` receiver calls.
- Full diagnostics: `analysis/STRICT_LINK_ARM32_BUILD_ATTEMPT.log`. This is a failed cross-target probe, not a successful Android build.

## Real Android NDK r27d compile and strict link — 2026-10-23

- Downloaded and SHA1-verified official Linux NDK r27d. Restored archive symlinks mangled by this environment's unzip; the Android API 21 ARMv7 compiler then ran normally.
- Assembled the current V21 overlay, compiled 46/46 translation units, and linked with `--no-undefined` against nine V21 ARM32 host libraries: PASS. `build_android.py` now selects static libc++ by default to avoid adding a direct `libc++_shared.so` dependency absent from the original. The shared-STL variant also linked successfully.
- Result is ELF32 ARMv7 Thumb-2/softfp, with the three named entrypoints. Its SHA256 is `5c0587af6f4edbc68d12001f459793bafcf4d07ca35e7011507eea1f7a6bd550`. See `analysis/ANDROID_NDK_R27D_BUILD.md` for commands and dependency comparison.
- This output is a linkable development build, not yet a drop-in replacement: 34 original dynamic exports are absent, the original has 23 direct dependencies versus 12 in the new image, and Fragment/Packet and AutoExscreen observer lifecycle recovery remains unfinished. No Android device load/runtime validation was done.

## Device black display and crash report — 2026-10-23

- The supplied `V21_ARM32_CRASH_19410.zip` confirms that the SO loads (`ret=1`) and GFrame immediately reports failure to load `InstrumentPannelLargePackage` of type `com.autonavi.package.GIPLPacket`; the same thread receives SIGABRT. No new usable tombstone identifies the abort PC.
- The original has five load-time initializers and the rebuilt image has none. The last original initializer registers the Packet factory. Original factory `0x2eb08` constructs the host package, CoverUtil, and four virtual subobjects before publishing the Packet singleton to BSS `0x6fd48`. This also identifies the missing LargeScreenImpl package receiver initializer. The current Packet's byte storage and constructor do not perform these operations, so automatically registering it would be unsafe.
- Added an ELF inspection gate for this original factory and its vtable offsets. This is an analysis checkpoint; there is no new device-safe replacement SO. See `analysis/DEVICE_BLACK_SCREEN_CRASH.md`, `analysis/v21_verified/packet_factory_runtime_table.md`, and `tools/inspect_packet_factory.py`.

## Packet singleton publication order — 2026-10-23

- Removed the partial `CGIPLPacket` constructor's premature singleton write. The original factory publishes only after host/CoverUtil construction and all four derived virtual pointers are installed.
- Added a private factory-only publication step. The existing shutdown contract test now simulates that step after preparing its mock host lock; it passes when compiled against the assembled overlay. This change does not create the host base, a live Packet factory, GFrame registration, or an installable SO.
- Verified the host `CGFragmentPackage(CGContext*)` constructor's imported `CGPackage` call and both vptr stores through its relocation tables. The original-ELF inspection gate now checks both instrument and host binaries.

## Unpublished Packet construction stages — 2026-10-23

- Added an owned `0x16c`-byte construction guard with the original host C2 call, CoverUtil placement construction at `+0xc0`, and four-byte proxy clear at `+0x168`. On ARM32 its operations bind to the matching GFrame C2/D2 exports and the local CoverUtil class. It tears down partial construction in reverse order and never publishes or registers the object.
- Host ASan/UBSan regression covers normal construction/destruction, failures in the host and Cover constructors, and the absence of singleton publication. These changes do not install Packet vptrs or implement the Fragment lifecycle; there is no new device-safe SO. Details: `analysis/PACKET_FACTORY_STAGING.md`.
- Re-read V21 CoverUtil D1 at `0x17dfc`: after optional logging it destroys the nested CGString at `+0x44`, then the String16 at `+0x18`. Default C++ member destruction has that order; the original does not release the image/observer in D1. Extended the original-ELF gate to pin those calls and the Packet-to-Cover destructor edge. The full lifecycle release remains separate and incomplete.

## Host and Packet virtual-table relationship — 2026-10-23

- Compared the matching host `CGFragmentPackage` 16-slot primary and two-slot `CGObjMem` virtual tables with Packet's four groups. Confirmed that Packet replaces lifecycle slots 6/9/10/14/15 while the host has short no-op bodies there. The two imported CGAsyncObject getters remain the same. Source-level return types for no-op slots remain unknown.
- Decoded six Packet destructor adjustor thunks: `-0x40`, `-0xc0`, `-0xc4` into D1/D0. The business observer keeps CoverUtil's separate `-4` callback thunk. Extended `tools/inspect_packet_factory.py --check-v21` to pin both ELF virtual tables and thunk bytes. See `analysis/PACKET_VIRTUAL_ABI.md`. The current Packet still has no derived virtual table or host class layout and must not be registered.
