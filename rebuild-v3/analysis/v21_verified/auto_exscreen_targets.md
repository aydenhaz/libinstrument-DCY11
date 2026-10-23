# AutoExscreen remaining callbacks

The V21 original is ARM32. This file records verified call paths and
implementation limits; a strict link alone cannot establish replacement
behavior.

| Entry | RVA | Current state |
| --- | ---: | --- |
| OnSetExternalScreenCrossControlUi(int,bool) | 0x2bcdc | Restored UI config write and CrossControl observer dispatch |
| OnSetExternalScreenMapStatusUi(int,int,int,int) | 0x2c070 | Not implemented |
| OnSetExScreenMapStatusUi(tagSTExtScreenControl const&) | 0x2ace0 | Not implemented |

## Map status entry at 0x2c070

Arguments after `this` are map engine ID, requested level, requested mode,
and a fourth integer on the stack. The fourth integer is only consumed by
the logging path in this function. It first calls
`HSL::GetMapViewHandle(mapEngineId)`; when null it leaves without changing
state. It looks up scene with screen type at `this+0xdc`, reads current
`AutoMultiScreenConfig` scene record, and copies its 0xe4-byte object at
0x17a6c. This is not a raw 0xe4-byte memcpy: three 12-byte string-like
subobjects at +0x54/+0x60/+0x6c use the copy constructor at 0x17b8e;
the temporary destroys all three at 0x17b54. Configured level and view
mode are at copied record offsets
+0x88 and +0x8c.

The level path checks 0x17ad0, and for requested level other than 99 calls
0x173fc (field 0 UI config) then 0x1e084 (instance SetMapLevel). For requested
mode other than 99 it calls 0x171b8 (field 1; preview scene 2 maps to scene 1),
then may invoke map-view `OpenDynamicLevel(false, type 0)` at slot +0x3e8,
conditionally start the timer
at `this+0xcc`, and call instance SetMapViewMode at 0x1def4. It finally
calls ResetMapBindingList at 0x1e790 using the scene ID. The current source's
Batch12's incorrect static SetMapLevel pseudo-implementation was withdrawn;
the real instance method remains undefined.
ResetMapBindingList now has the observed binding-state and service dispatch,
but its containing singleton and scene-record constructor remain missing.
These dependencies need repair before installing the callback.

The scene slots and UI-config read/write wrappers are now reconstructed
in `AutoExscreenUiConfig.cpp`. `AutoExscreenMapStatusPlan.h` records the
verified 99 / -1 decision branches for subsequent callback integration. The
ARM32 Linux probe still leaves this callback undefined until its config
singleton, timer and map-view dispatch are restored.

The dynamic branch is narrower than the predicate alone: requested mode
must differ from 99 and requested level must differ from -1. Thus its
map-view `OpenDynamicLevel(false, type 0)` and guarded `CGUITimer::start` happen when the
configured level is -1 and requested level is 99. The timer object comes
from observer +0xcc. It is started only when its +4 word is at least 1,
its first two bytes are clear and `start` returns true; the callback then
sets its first byte. Mode update and final `ResetMapBindingList(scene)`
follow. The current `MapStatusPlan` records these decisions, not the
timer's unimplemented object ABI.

## Control record entry at 0x2ace0

The source withdrew a wrongly assigned body. The original is 3788 bytes,
accepts a 36-byte tagSTExtScreenControl record and drives several screen
state transitions. Its branches, concrete object ownership, and downstream
callbacks still require recovery.

### Verified 0x2ace0 branches

- `word[0]` is mapped to an engine by `MultiScreenService`; a null map-view
  handle exits without applying status. On the fast path, that engine must
  equal observer +0xe0 and be 3, 5 or 7. `word[6]` must equal current mode+1
  or 99. Failure of any comparison leaves through the diagnostic path at
  0x2af60, before UI config writes and HMI status output.
- `word[1]` other than 99 writes UI field 1 and calls the instance
  `SetMapViewMode`; `word[2]` other than 99 writes UI field 0 and calls
  `SetMapLevel`. Values -2 and -3 request a step up/down based on the
  current map-view scale from virtual slot +0x198, with upper/lower guards
  at 18 and 4. The normal value is written directly. The sentinel -2 or -3
  still reaches instance `SetMapLevel` after the UI config receives the
  calculated absolute level. At the guard the instance call is skipped.
- `word[3]` other than 99 invokes CrossControl with the predicate
  `word[3] == 1`. `word[4]` and `word[5]` affect guide UI fields 3 and 4
  respectively only when their values are 0 or 1. Field 4 is written to
  all three scene IDs and observers in the collection at +0x90 are called.
- For field 3, `ControlAllMultiModeFlag` is read from `hsl::Config` and its
  lowest bit tested. When set, `AutoConfigJsonData::GetMultiViewModeInfo`
  supplies one byte per mode. The callback writes guide field 3 for each
  returned mode index, using that byte if `word[4] == 1` or zero otherwise.
  When clear, it writes only the current mode, with 1 or 0. This branch is
  extracted into `AutoExscreenUtil_SetEagleEyeEnabledToUiConfig`; the original
  host getter returns bool in r0, contrary to the former `void` stub.
- After `ResetMapBindingList(currentScene)`, `word[8] == 99` skips the longer
  map-style branch. Value 2 selects the string-like scene member at +0x54;
  value 1 selects +0x60. A nonempty name invokes
  `SetSpecifiedMapStyle(true)` and `SetMapStyleName(name)`; an empty name
  invokes `SetSpecifiedMapStyle(false)`. Both call `SetMapStyleTime` with
  a first argument of 0 (value 2) or 1 (value 1), followed by three false
  flags. Value 0 first checks `GetDeviceId` / `IsDeviceAttach`, uses
  `GetMapThemeStyle` when attached and `GetNightMode` as a fallback, then
  prefers the +0x60 style name only when the resulting value is 1 and that
  name is nonempty, otherwise tries +0x54, then calls `SetMapStyleTime`
  with the resulting value.
  The selector's external protocol name remains unverified. Unsupported
  values skip these map-style calls.
- The trailing path copies the **entire input control record** to a 36-byte
  status buffer at 0x2b872, overwrites words 0–6 through 0x2be04, and
  passes that buffer to `GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl`
  at 0x2bb2c. Words 7 and 8 thus retain the input values. Word 0 is the
  window ID of the observer's engine; word 1 is the singleton's +0x314
  view mode; word 2 is `int(GetZoomLevel())`; word 3 gates the UI config
  CrossControl value on scene +0x9c; word 4 combines scene +0xc7 with
  UI config EagleEye; word 5 reads UI config preview via 0x17180; and
  word 6 is current screen mode + 1. The original status logic also maps
  `GetMapMode()` into a provisional view-mode field before 0x2be04
  overwrites it on a successful map-view lookup.
  The exact external meanings of fields 3–5 still require validation.

`map_view_virtuals.md` records the matching host vtable symbol names and
machine-code call sites. These slot signatures depend on the V21 host ABI.

The two callbacks still lack the full constructed config singleton,
`SetMapLevel`, `SetMapViewMode`, map-view dispatch and observer
construction. The partial helper is not installed as a substitute callback.

The observer constructor at 0x29798 and registrations are also incomplete.
The CrossControl member implementation assumes its original object layout;
it has not been exercised on a constructed device observer.

The three scheduling entries at 0x2a5a8, 0x2a7e0 and 0x2aa28 bind the above
callbacks with captured objects of 24, 32 and 52 bytes respectively. The
source's previous zero-argument ActionA/B/C methods were invented. The
reconstructed GPI Callable and UIObject member import clear those incorrect
link errors in the ARM32 Linux probe. Its source token is currently zero,
and scheduling/cancellation has not been validated on Android.
