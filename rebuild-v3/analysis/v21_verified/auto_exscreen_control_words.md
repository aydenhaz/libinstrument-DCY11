# V21 external-screen control record

Original `libInstrumentPannelLarge.so` SHA-256:
`6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3`.
The record passed to callback 0x2ace0 occupies 36 bytes. In the known
construction path it starts with one zero followed by eight 99 sentinels.
This table follows register/data flow in the original callback; names are
descriptive labels for the reconstructed C++ record, not recovered debug names.

| Word / offset | Verified use in callback 0x2ace0 |
| --- | --- |
| 0 / +0x00 | Passed to `MultiScreenService::GetMapEngineId(int)` at 0x2ad46. |
| 1 / +0x04 | Unless 99, passed to UI mode writer 0x171b8 and to instance `SetMapViewMode` 0x1def4 at 0x2af04–0x2af18. |
| 2 / +0x08 | Unless 99, used for level changes, then passed to instance `SetMapLevel` 0x1e084 at 0x2b0e6. -2 and -3 select relative steps. |
| 3 / +0x0c | Unless 99, compared with 1 and dispatched to `OnSetExternalScreenCrossControlUi` at 0x2b102. |
| 4 / +0x10 | Values 0 or 1 are saved to field 3 of the guide-scene UI configuration at 0x2b1b4 and sent to a collection of observers. |
| 5 / +0x14 | Values 0 or 1 are saved to field 4 of the UI configuration for three scenes at 0x2b262–0x2b27a and sent to a separate collection. |
| 6 / +0x18 | Compared with current screen mode + 1 or 99 at 0x2ad80; selects an early path. |
| 7 / +0x1c | No direct load identified in this callback; retained unchanged. |
| 8 / +0x20 | Checked against 99 at 0x2b2d2 and branches into a longer map-view/scene path. Its user-facing meaning is unverified. |

The fast path requires a nonnull map-view handle, the expected engine ID at
observer +0xe0, and a matching screen-mode condition. Its path copies the
0xe4-byte scene record, applies fields 1–4, then calls
`ResetMapBindingList(scene)`. The remainder of this 3788-byte callback
contains map-view virtual calls and scene changes. A complete implementation
still needs those calls, the scene record ownership and the config singleton.

Separate wrappers 0x173fc and 0x171b8 now persist requested level to the
current scene and requested view mode to the current scene, except that a
preview-scene view mode is written under the guide-scene key. Both read
`AutoConfigScreenMode::GetCurrentScreenMode(screen)` and then use the verified
Bundle writer 0x168e8. V21 0x1984c returns three initially zero scene slots,
falling back to slot 0 for unexpected screen selectors.
