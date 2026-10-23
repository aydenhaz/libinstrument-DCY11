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

The `AutoMultiScreenConfig` constructor / singleton, `SetMapLevel`, and
`SetMapViewMode` still need restoration before this method can be safely
invoked from the unfinished map-status callback. A successful strict link
would still not prove that the library can replace the original.

The map-view lookup at 0x1ddd8 is now reconstructed as an isolated helper.
It reads screen type from `instance+0x30c`, calls the original 0x16788
screen-to-engine mapping (Large→5, Small→3, Widget→7), rejects other
selectors, and passes the engine to `HSL::GetMapViewHandle`. These three
values come from the original table at 0x5d158. A host ASan/UBSan test
checks the three calls and invalid selector paths. The original also logs
on invalid selectors; the helper does not yet reproduce that diagnostic.

Two constructor leaf functions are also restored without constructing the
full singleton. V21 0x1c210 writes the default values and zero ranges of
each 0xe4-byte scene record, preserving untouched bytes; it initializes the
four selected words to -1, stores 6 and 240 at +0x38/+0x3c, and sets the
floating-point sentinels at their verified offsets. V21 0x3bec0 zeros binding bytes
+4..+0x1c and writes the engine 5/3/7 (or -1) at word zero. The containing
0x340-byte constructor at 0x1c174 also initializes a lock and registers
three scenes at 0x1c2d4. Those side effects and singleton lifetime remain
unimplemented, so these helpers must not be used as a complete constructor.
