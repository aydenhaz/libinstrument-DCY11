# V21 external-screen UI config writer

Verified against the ARM32 original, SHA-256
`6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3`.

The writer at 0x168e8 takes (ScreenType, mode, scene, field, value), reads a
root hsl::Bundle, and writes a rebuilt root through
`hsl::Config::SetBundle(key, bundle, SetterType_(1))`. Each record has
20 modes, three scenes per mode, six signed 32-bit fields per scene. Missing
fields are initialized to 99; the target field alone gets the new value.

| Level | Index | Literal key |
| --- | ---: | --- |
| Root | 0 / 1 / 2 | ExScreenLarge / ExScreenSmall / ExScreenWidget |
| Mode | 0 through 19 | kExScreenModeKey + decimal index |
| Scene | 0 / 1 / 2 | kExScreenCuriseKey / kExScreenGuideKey / kExScreenPreviewKey |
| Field | 0 | int32MapLevel |
| Field | 1 | int32MapViewMode |
| Field | 2 | int32CrossControl |
| Field | 3 | int32EagleEyeEnable |
| Field | 4 | int32MapPreviwe |
| Field | 5 | int32mapDayNightMode |

The misspellings and lowercase `map` are original binary strings. The
implementation guards indices before accessing the key arrays and preserves
previous fields in other modes/scenes. This is a host-contract test of Bundle
calls, not an Android process test.

The original reader at 0x16fcc obtains the screen bundle, selects one of
20 mode records and three scene records through 0x1c0bc, then returns one
of the six int32 values. It returns 99 when the configuration handle is null,
the screen/mode/scene/field is invalid, or the requested record is absent.
`AutoExscreenUiConfig.cpp` now implements that lookup alongside the writer.
The 40-byte wrappers 0x1748c, 0x17320 and 0x17348 query the current mode and
scene and then read fields 0 (level), 1 (view mode), and 5 (day/night).
Host tests cover both directions, absent config and invalid indices.

The separate selector 0x174b4 reads `IsLargePannelOnlyShowCrossImage`,
`IsSmallPannelOnlyShowCrossImage` or `IsWidgetOnlyShowCrossImage` from
`Config::GetInt32`, based on screen type. It uses its second argument as the
fallback when the value is absent/99 or the config handle is unavailable.
This selector feeds the unrecovered binding-state calculation at 0x1e790.

At 0x2bcdc CrossControl uses `this+0xdc` for screen type, current mode,
scene 1/field 2, then locks the collection at `this+0x6c`. The collection
holds observer pointer array at +0x7c and count at +0x84. Its lock/unlock
vtable slots are +8/+12. Each observer receives (screen type, enabled) in
its first virtual slot.

At 0x17ad0 the dynamic-level condition is
`requestedLevel == -1 || (configuredLevel == -1 && requestedLevel == 99)`.

At 0x2b106 the EagleEye control word 0/1 enters a second UI writer path.
`ControlAllMultiModeFlag` (UTF-16 key) selects all modes when its lowest bit
is set; a successful `GetMultiViewModeInfo` returns one byte per mode.
Guide scene (1), field 3 is written with that byte for enable, or zero for
disable. When the flag is clear or absent, only the current mode is written
with one or zero. Host ASan/UBSan tests cover both paths and a failed mode
lookup. `GetMultiViewModeInfo` returns bool in r0 in V21 `libCommon.so`;
its previous void declaration matched the mangled symbol but lost this
control-flow result. This helper is not the whole map status callback.
