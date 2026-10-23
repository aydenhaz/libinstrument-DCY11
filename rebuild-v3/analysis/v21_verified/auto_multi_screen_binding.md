# V21 map binding reconstruction

Reference: V21 ARM32 `libInstrumentPannelLarge.so`, SHA-256
`6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3`.

At 0x1e790, `AutoMultiScreenConfig::ResetMapBindingList(scene)` chooses a
0xe4-byte scene record at instance offsets 0, 0xe4, or 0x1c8, and a 0x20-byte
binding record at +0x2ac, +0x2cc, or +0x2ec. Unknown scene IDs use the
first record. Screen type is stored at instance +0x30c; word 0 in the
binding record is an engine selector. The original maps engine selector
3 to Small, 7 to Widget and the rest to Large before reading the per-screen
OnlyShowCrossImage override.

The method reads current-mode/current-scene UI fields 5 (day/night), 0
(level), and 1 (view mode), in that order. It resolves scene record fields
at +0x4c, +0x50, +0x88, +0x8c, +0x90, +0x99, and +0xa0 into binding words
at +4 through +0x18. `AutoMultiScreenBindingState.h` contains the verified
selection, sentinel and clamp branches. Its host test covers dynamic mode,
level boundaries, 99 fallbacks, and the preserved engine/bound-state words.

After writing the binding state, the original obtains
`HSL::GetMapServiceHandle()`, calls virtual slot +0x3c twice with selector
1 and with binding word 0, and calls virtual slot +0x3c on the first returned
object with the second object and `binding+4`. It sets the byte at binding
+0x1c to 1 just before the final call. The reconstructed dispatcher now
follows this order. A mock host test covers success, missing second object
and missing service. These are ABI contract checks; the actual map service
and live scene objects have not been exercised on Android.

The `AutoMultiScreenConfig` constructor / singleton now has a reconstructed
ARM32 path with per-screen lazy initialization and imported scene reads.
The instance `SetMapViewMode` and `SetMapLevel` methods use the verified V21
map-view virtual slots and ARM register arguments. The isolated dynamic
initializer reconstructs the
0x88-byte parameter record and +0x3cc/+0x3e8/+0x3f0 virtual-call sequence
from V21 0x1765c. It is not the original singleton-owning static function.
A successful strict link would not itself prove that the library can replace
the original.

The map-view lookup at 0x1ddd8 is now reconstructed as an isolated helper.
It reads screen type from `instance+0x30c`, calls the original 0x16788
screen-to-engine mapping (Large→5, Small→3, Widget→7), rejects other
selectors, and passes the engine to `HSL::GetMapViewHandle`. These three
values come from the original table at 0x5d158. A host ASan/UBSan test
checks the three calls and invalid selector paths. The original also logs
on invalid selectors; the helper does not yet reproduce that diagnostic.

The shorter map-status callback at 0x2c070 deep-copies one scene record but
subsequently reads only words +0x88 and +0x8c from the copy. The isolated
`SnapshotMapState` helper reads those words in the same order before any
updates. A full `SceneConfig` copy would have to deep-copy three owning
12-byte strings at +0x54/+0x60/+0x6c; a raw 0xe4-byte `memcpy` is invalid.

Two constructor leaf functions are used by the reconstructed singleton.
V21 0x1c210 writes the default values and zero ranges of
each 0xe4-byte scene record, preserving untouched bytes; it initializes the
four selected words to -1, stores 6 and 240 at +0x38/+0x3c, and sets the
floating-point sentinels at their verified offsets. V21 0x3bec0 zeros binding bytes
+4..+0x1c and writes the engine 5/3/7 (or -1) at word zero. The containing
0x340-byte constructor at 0x1c174 also initializes a lock and registers
three scenes at 0x1c2d4. The leaf helpers alone do not construct the lock,
populate JSON scene data, or establish per-screen lifetime; the singleton
source now supplies those steps on ARM32.

Constructor call sequence from 0x1c174/0x1c2d4: initialize scene records at
+0, +0xe4 and +0x1c8, then initialize bindings at +0x2ac, +0x2cc and
+0x2ec with the same screen argument. Store the screen at +0x30c, zero
+0x310, set +0x314 to 3, construct an `asl::LockLinux` wrapper at +0x318,
set +0x338 to -1 and clear +0x33c. While holding that lock, call
`AutoConfigJsonData::GetInstance()` and its imported const
`GetSceneConfig(ScreenType, SceneType, SceneConfig&)` three times for scene
IDs 0, 1 and 2, with each corresponding 0xe4-byte record as the output.
The call targets were verified through the original GOT relocations at
0x6e950 and 0x6e954. `AutoMultiScreenConfigSingleton.cpp` now follows this
order, including the wrapper's exported `asl::Mutex` address point +8,
`LockLinux` construction at instance+0x31c, virtual lock/unlock at wrapper
+0x318 and three imported `GetSceneConfig` calls. Its scene strings use
ARM32 libc++ `std::string`, whose 12-byte size and four-byte alignment are
compile-time checked. Lazy statics produce three 0x340-byte ARM32 instances.
The destructor destroys LockLinux and the scene strings in reverse order.
This ABI is not yet validated in a running Android process.
