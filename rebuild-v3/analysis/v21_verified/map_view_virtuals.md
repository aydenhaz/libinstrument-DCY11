# V21 map view virtual calls in external-screen status

Reference instrument DSO: `libInstrumentPannelLarge.so` SHA-256
`6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3`.
Reference host: `host_libs/libhsl.so` SHA-256
`79c98162a024c0778971b950d571a4787aeb1c22bc2f77031039c8d3371c0194`.

`libhsl.so` exports `_ZTVN3hsl15MapViewBaseImplE` at 0x690f24. Its ARM32
virtual table address point is at +8. Reading `.rel.dyn` at address point
plus each byte offset gives the following symbol names. This verifies the
slot contract for `MapViewBaseImpl`; a derived map view can override a slot.

| Slot | Host virtual method | Instrument call sites |
| ---: | --- | --- |
| +0x88 | `MapZoomIn(bool,bool,bool,bool)` | `SetMapLevel` 0x1e1d4 |
| +0x8c | `MapZoomOut(bool,bool,bool,bool)` | `SetMapLevel` 0x1e122 |
| +0x90 | `SetMapMode(bl::MapviewMode,bool,bool,bool,bool)` | `SetMapViewMode` 0x1dfec / 0x1e048 |
| +0x98 | `GetMapMode()` | control callback 0x2ba88 / 0x2bad4 |
| +0xa8 | `GetDeviceId()` | control callback 0x2b456 |
| +0x14c | `SetMapStyleName(char const*)` | 0x2b664 / 0x2b6e2 / 0x2b812 |
| +0x150 | `SetSpecifiedMapStyle(bool)` | 0x2b640 / 0x2b6be / 0x2b7f6 |
| +0x158 | `SetMapStyleTime(bl::MapStyleTime,bool,bool,bool)` | 0x2b836 / 0x2b858 |
| +0x168 | `GetMapThemeStyle(bl::MapStyleParam&)` | 0x2b4b8 |
| +0x16c | `GetNightMode()` | 0x2b72a |
| +0x198 | `GetZoomLevel()` | control callback 0x2af38 / 0x2b030; status 0x2be9c |
| +0x1b0 | `SetZoomLevel(float,bool,bool,bool,bool)` | `SetMapLevel` 0x1e25c / 0x1e3c2 |
| +0x3cc | `InitDynamicLevel(bl::DynamicLevelParam const&)` | initializer 0x1775c |
| +0x3d4 | `ResetDynamicLevel()` | `SetMapLevel` 0x1e352 |
| +0x3e8 | `OpenDynamicLevel(bool,bl::DynamicLevelType)` | simple callback 0x2c19c; `SetMapLevel` 0x1e23a / 0x1e2fe / 0x1e31a / 0x1e346 |
| +0x3ec | `IsOpenDynamicLevel(bl::DynamicLevelType)` | scene selector 0x1783e / 0x17894 |
| +0x3f0 | `OpenDynamicCenter(bool)` | initializer 0x1777a |
| +0x430 | `IsDeviceAttach(bl::EGLDeviceID)` | control callback 0x2b462 |

## Behavior relevant to the two missing callbacks

In 0x2c070 the dynamic-level path calls `OpenDynamicLevel(false, 0)` at
0x2c196–0x2c1a2, then conditionally starts the timer. It does **not**
call `ResetDynamicLevel()`. The latter appears separately inside the
instance `SetMapLevel` implementation at 0x1e352.

In 0x2ace0, control word 8 selects map style. Value 2 uses the scene
record's nontrivial string at +0x54 and `SetMapStyleTime(0,false,false,false)`;
value 1 uses the string at +0x60 and
`SetMapStyleTime(1,false,false,false)`. If the selected string is empty,
only `SetSpecifiedMapStyle(false)` is called before setting the time.
Value 0 obtains `GetDeviceId` from a separate map view, checks
`IsDeviceAttach`, then fills a map-style parameter via `GetMapThemeStyle`
or falls back to `GetNightMode`. If the resulting value is 1 and the
+0x60 name is present, that name is used; otherwise the +0x54 name is
used when present. `SetMapStyleTime` receives the resulting value. The
scene string representation has a short/heap bit;
passing the address of the 12-byte string object as `char const*` is invalid.

`SetMapViewMode` at 0x1def4 requires both the current screen's map view and
engine 1's reference view. It chooses `requestedMode` unless it is 99,
in which case it uses `fallbackMode`. After both lookups succeed it stores
that value at singleton +0x314. Values 0, 1 and 2 dispatch
`SetMapMode(2 - value, false, false, false, false)` on the screen view;
value 3 dispatches the reference view's `GetMapMode()` result; other
values store the value but make no map-view call. The instance method and
an independently callable raw-layout helper now restore these branches.

`SetMapLevel` at 0x1e084 requires the same two views and uses the following
ARM32 jump table (`requestedLevel + 3` at 0x1e108):

| Requested level | Observed path |
| --- | --- |
| -3 | Close the opposing scene's dynamic type if open, then `MapZoomOut(true,false,true,false)`. |
| -2 | Same scene check, then `MapZoomIn(true,false,true,false)`. |
| -1 | Close the previous dynamic type if +0x338 is active; initialize dynamic parameters at 0x1765c; store scene==0 at +0x338. |
| 0 | Close opposing dynamic type, read reference `GetZoomLevel`, set the same float level with flags (false,false,true,false), then close the previous +0x338 type if active. |
| 3 through 19 | Close the previous +0x338 type if active, close opposing scene type, then `SetZoomLevel(float(requested),false,true,false,false)`. |
| 99 | Use the fallback argument: -1 follows dynamic initialization, 0 preserves reference zoom, all other values clamp to 3..19 and set zoom with flags (false,true,true,false). This last case also calls `ResetDynamicLevel()` if +0x338 was active. |

Other request values leave without applying a level. At 0x1781c,
scene 0 checks `IsOpenDynamicLevel(1)` before `OpenDynamicLevel(false,1)`;
scene 1 similarly checks and closes type 0. This operation is restored as
an isolated helper. Calls that close the previous +0x338 type pass the
**stored type** as the second `OpenDynamicLevel` argument, not a constant
zero. The 0x1765c initializer's map-view dispatch is now reconstructed as
an isolated raw-layout helper: it reads scene word +8, converts it to a
quarter integer, writes the observed 0x88-byte parameter object, and calls
`InitDynamicLevel`, `OpenDynamicLevel(true, scene==0)` and
`OpenDynamicCenter(false)` in that order. Parameter word +0x44 is 45.0f,
while +0x50 is 30.0f. Its original static wrapper, singleton lookup and
`SetMapLevel` are still absent. The old receiver-free static pseudo-method
has been removed.

The previous Batch13 placeholder assigned 0x2be04 to
`AutoMultiScreenConfig::GetMapViewModeConfig()` and returned an empty class.
The actual caller passes the observer and a 36-byte status output pointer;
that synthetic method has now been removed.

At 0x2b872 the control callback copies 36 bytes from its input into an
outgoing status record. It can adjust the copied mode and level using
`GetMapMode` and `GetZoomLevel` at 0x2ba86–0x2bae6 and 0x2b8ee–0x2b9d0.
When the observer's map view exists, 0x2be04 then replaces words 0–6:

| Word | Source in 0x2be04 |
| ---: | --- |
| 0 | `MultiScreenService::GetWindowId(observer+0xe0)` |
| 1 | singleton +0x314, the effective map view mode |
| 2 | integer conversion of `GetZoomLevel()` |
| 3 | normalized UI config CrossControl for the current scene when scene +0x9c is nonzero, otherwise 0 |
| 4 | EagleEye UI config field 3, falling back to scene byte +0xc7 when it is 99; normalized to 0/1 |
| 5 | preview UI config field 4 through 0x17180, preserving 0/1 and mapping other values to 99 |
| 6 | current screen mode + 1 |

Words 7 and 8 retain the copied input. If the map view is unavailable when
0x2be04 runs, that function returns without replacing words 0–6, so the
earlier mode/level adjustments can still matter. The initial entry gate
already required a view for the same engine; this second lookup could
still observe a changed host state. The callback sends the resulting record
to `GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl` at 0x2bb2c.

This analysis identifies method names and call order. It does not
establish that the current source's `hsl::MapView` declaration has the
full Android object ABI or that the whole callback is safe to install.
